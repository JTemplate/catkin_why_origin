#include "sensorfusion.h"

#include <cinttypes>
#include <cmath>

#include <rclcpp/rclcpp.hpp>

namespace {

// Constant-velocity model driven by white acceleration noise.
//
// 3 m/s^2 standard deviation -> variance = 9 (m/s^2)^2.
// These are model tuning parameters, not measurement noise.


Eigen::MatrixXd processNoiseForDeltaTime(
    double delta_t,
    double acceleration_variance_x,
    double acceleration_variance_y)
{
    const double dt2 = delta_t * delta_t;
    const double dt3 = dt2 * delta_t;
    const double dt4 = dt3 * delta_t;

    Eigen::MatrixXd Q = Eigen::MatrixXd::Zero(4, 4);

    Q <<
        0.25 * dt4 * acceleration_variance_x,
        0.0,
        0.5 * dt3 * acceleration_variance_x,
        0.0,

        0.0,
        0.25 * dt4 * acceleration_variance_y,
        0.0,
        0.5 * dt3 * acceleration_variance_y,

        0.5 * dt3 * acceleration_variance_x,
        0.0,
        dt2 * acceleration_variance_x,
        0.0,

        0.0,
        0.5 * dt3 * acceleration_variance_y,
        0.0,
        dt2 * acceleration_variance_y;

    return Q;
}

}  // namespace

bool ShouldProcessLidarFirst(
    std::int64_t lidar_timestamp,
    std::int64_t radar_timestamp)
{
    return lidar_timestamp <= radar_timestamp;
}

SensorFusion::SensorFusion()
  : SensorFusion(SensorFusionConfig{})
{
}

SensorFusion::SensorFusion(
    const SensorFusionConfig & config)
  : process_acceleration_variance_x_(
        config.process_acceleration_variance_x),
    process_acceleration_variance_y_(
        config.process_acceleration_variance_y)
{
    is_initialized_ = false;
    last_timestamp_ = 0;

    H_lidar_ = Eigen::MatrixXd(2, 4);
    H_lidar_ << 1, 0, 0, 0,
                0, 1, 0, 0;

    R_lidar_ = Eigen::MatrixXd(2, 2);
    R_lidar_ <<
        config.lidar_measurement_variance_x, 0,
        0, config.lidar_measurement_variance_y;

    R_radar_ = Eigen::MatrixXd(3, 3);
    R_radar_ <<
        config.radar_measurement_variance_range, 0, 0,
        0, config.radar_measurement_variance_bearing, 0,
        0, 0, config.radar_measurement_variance_range_rate;
}


SensorFusion::~SensorFusion()
{

}

void SensorFusion::SetRadarExtrinsic(
    double translation_x,
    double translation_y,
    double yaw)
{
    radar_translation_x_ = translation_x;
    radar_translation_y_ = translation_y;
    radar_yaw_ = yaw;

    kf_.SetRadarExtrinsic(
        translation_x,
        translation_y,
        yaw);
}


bool SensorFusion::Process(const MeasurementPackage & measurement_pack)
{
    // 第一帧数据用于初始化 Kalman 滤波器
    if (!is_initialized_) {
        Eigen::Vector4d x = Eigen::Vector4d::Zero();

        if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
            // LiDAR only provides position for initialization.
            x << measurement_pack.raw_measurements_[0],
                 measurement_pack.raw_measurements_[1],
                 0.0,
                 0.0;

        } else if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
            const double rho = measurement_pack.raw_measurements_[0];
            const double phi = measurement_pack.raw_measurements_[1];
            const double rho_dot = measurement_pack.raw_measurements_[2];

            const double cos_phi = std::cos(phi);
            const double sin_phi = std::sin(phi);

            // Radar measurement expressed in the Radar frame.
            const double position_x_radar = rho * cos_phi;
            const double position_y_radar = rho * sin_phi;

            const double velocity_x_radar = rho_dot * cos_phi;
            const double velocity_y_radar = rho_dot * sin_phi;

            // Transform Radar-frame position and velocity into
            // the velodyne frame using the Radar extrinsic.
            const double cos_yaw = std::cos(radar_yaw_);
            const double sin_yaw = std::sin(radar_yaw_);

            const double position_x =
                radar_translation_x_ +
                cos_yaw * position_x_radar -
                sin_yaw * position_y_radar;

            const double position_y =
                radar_translation_y_ +
                sin_yaw * position_x_radar +
                cos_yaw * position_y_radar;

            // Translation does not affect velocity.
            const double velocity_x =
                cos_yaw * velocity_x_radar -
                sin_yaw * velocity_y_radar;

            const double velocity_y =
                sin_yaw * velocity_x_radar +
                cos_yaw * velocity_y_radar;

            x << position_x,
                 position_y,
                 velocity_x,
                 velocity_y;
        }

        // Avoid a state exactly at the origin, where the Radar
        // measurement Jacobian becomes singular.
        if (std::fabs(x(0)) < 0.001) {
            x(0) = 0.001;
        }

        if (std::fabs(x(1)) < 0.001) {
            x(1) = 0.001;
        }

        // 初始化 Kalman 滤波器
        kf_.Initialization(x);

        // 设置协方差矩阵 P
        Eigen::MatrixXd P = Eigen::MatrixXd(4, 4);
        P << 1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1000.0, 0.0,
             0.0, 0.0, 0.0, 1000.0;
        kf_.SetP(P);

        // No prediction has happened yet. Process noise is derived
        // from the real prediction interval immediately before each
        // Prediction() call.
        kf_.SetQ(Eigen::MatrixXd::Zero(4, 4));

        // 存储第一帧时间戳
        last_timestamp_ = measurement_pack.timestamp_;
        is_initialized_ = true;

        return true;
    }

    // Reject measurements older than the last accepted measurement.
    if (measurement_pack.timestamp_ < last_timestamp_) {
        RCLCPP_WARN(
            rclcpp::get_logger("sensor_fusion"),
            "Dropping out-of-order measurement: current=%" PRId64
            " ns, last=%" PRId64 " ns, lag=%.3f ms",
            static_cast<std::int64_t>(measurement_pack.timestamp_),
            last_timestamp_,
            (last_timestamp_ - measurement_pack.timestamp_) / 1000000.0);

        return false;
    }

    // 时间戳单位从 ns 转换成 s
    const double delta_t =
        (measurement_pack.timestamp_ - last_timestamp_) /
        1000000000.0;

    RCLCPP_DEBUG(
        rclcpp::get_logger("sensor_fusion"),
        "last_timestamp_: %" PRId64 ", current_time: %" PRId64,
        last_timestamp_,
        static_cast<std::int64_t>(measurement_pack.timestamp_));

    last_timestamp_ = measurement_pack.timestamp_;

    // 同一时间戳的 Radar / LiDAR measurement 只更新，
    // 不重复执行 prediction 和注入过程噪声。
    if (delta_t > 0.0) {
        Eigen::MatrixXd F = Eigen::MatrixXd(4, 4);

        F << 1.0, 0.0, delta_t, 0.0,
             0.0, 1.0, 0.0, delta_t,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0;

        kf_.SetF(F);
        kf_.SetQ(processNoiseForDeltaTime(
            delta_t,
            process_acceleration_variance_x_,
            process_acceleration_variance_y_)
);
        kf_.Prediction();
    }

    // Measurement update
    if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
        kf_.SetH(H_lidar_);
        kf_.SetR(R_lidar_);
        kf_.KFUpdate(measurement_pack.raw_measurements_);

    } else if (
        measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
    {
        kf_.SetR(R_radar_);

        // Radar Jacobian and Radar extrinsic geometry are handled
        // inside EKFUpdate().
        kf_.EKFUpdate(measurement_pack.raw_measurements_);
    }

    return true;
}


bool SensorFusion::Predict(std::int64_t timestamp)
{
    if (timestamp < last_timestamp_) {
        RCLCPP_WARN(
          rclcpp::get_logger("sensor_fusion"),
          "Skipping out-of-order prediction: current=%" PRId64
          " ns, last=%" PRId64 " ns, lag=%.3f ms",
          timestamp, last_timestamp_, (last_timestamp_ - timestamp) / 1000000.0);
        return false;
    }
    if (timestamp == last_timestamp_) {
        return true;
    }

    double delta_t = (timestamp - last_timestamp_) / 1000000000.0; // unit : s
    last_timestamp_ = timestamp;
    // 设置状态转移矩阵F
    Eigen::MatrixXd F = Eigen::MatrixXd(4, 4);
    F << 1.0, 0.0, delta_t, 0.0,
         0.0, 1.0, 0.0, delta_t,
         0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 1.0;

    kf_.SetF(F);
    kf_.SetQ(processNoiseForDeltaTime(
        delta_t,
        process_acceleration_variance_x_,
        process_acceleration_variance_y_)
    );
    kf_.Prediction();
    return true;
}

std::int64_t SensorFusion::LastTimestamp() const
{
    return last_timestamp_;
}

bool SensorFusion::ProcessRadarCartesian(
    double x_velodyne,
    double y_velodyne,
    double radial_velocity,
    std::int64_t timestamp)
{
    const double cos_yaw = std::cos(radar_yaw_);
    const double sin_yaw = std::sin(radar_yaw_);

    // Translate the velodyne-frame position so that the Radar
    // origin becomes the coordinate origin.
    const double dx =
        x_velodyne - radar_translation_x_;
    const double dy =
        y_velodyne - radar_translation_y_;

    // Rotate from the velodyne frame into the Radar frame:
    // p_radar = R^T * (p_velodyne - t)
    const double x_radar =
        cos_yaw * dx +
        sin_yaw * dy;

    const double y_radar =
        -sin_yaw * dx +
        cos_yaw * dy;

    const double rho =
        std::hypot(x_radar, y_radar);

    const double phi =
        std::atan2(y_radar, x_radar);

    MeasurementPackage measurement;
    measurement.sensor_type_ = MeasurementPackage::RADAR;
    measurement.raw_measurements_ = Eigen::VectorXd(3);

    measurement.raw_measurements_ <<
        rho,
        phi,
        radial_velocity;

    measurement.timestamp_ = timestamp;

    return Process(measurement);
}
