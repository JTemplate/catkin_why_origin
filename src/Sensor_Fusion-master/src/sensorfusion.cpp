#include "sensorfusion.h"

#include <cinttypes>
#include <cmath>

#include <rclcpp/rclcpp.hpp>


SensorFusion::SensorFusion()
{
    is_initialized_ = false;
    last_timestamp_ = 0;

    // 初始化激光雷达的测量矩阵 H_lidar_
    // Set Lidar's measurement matrix H_lidar_
    H_lidar_ = Eigen::MatrixXd(2, 4);
    H_lidar_ << 1, 0, 0, 0,
                0, 1, 0, 0;

    // 设置传感器的测量噪声矩阵，一般由传感器厂商提供，如不提供，也可通过有经验的工程师调试得到
    // Set R. R is provided by Sensor supplier, in sensor datasheet
    // set measurement covariance matrix
    R_lidar_ = Eigen::MatrixXd(2, 2);
    R_lidar_ << 0.0225, 0,
                0, 0.0225;

    // Measurement covariance matrix - radar
    R_radar_ = Eigen::MatrixXd(3, 3);
    R_radar_ << 0.09, 0, 0,
                0, 0.0009, 0,
                0, 0, 0.09;
}

SensorFusion::~SensorFusion()
{

}

bool SensorFusion::Process(const MeasurementPackage & measurement_pack)
{
    // 第一帧数据用于初始化 Kalman 滤波器
    if (!is_initialized_) {
        Eigen::Vector4d x = Eigen::Vector4d::Zero();
        if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
            // 如果第一帧数据是激光雷达数据，没有速度信息，因此初始化时只能传入位置，速度设置为0
            x << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0;
        } else if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
            // 如果第一帧数据是毫米波雷达，可以通过三角函数算出x-y坐标系下的位置和速度
            float rho = measurement_pack.raw_measurements_[0];
            float phi = measurement_pack.raw_measurements_[1];
            float rho_dot = measurement_pack.raw_measurements_[2];

            float position_x = rho * cos(phi);
            if (std::fabs(position_x) < 0.0001) {
                position_x = 0.0001;
            }

            float position_y = rho * sin(phi);
            if (std::fabs(position_y) < 0.0001) {
                position_y = 0.0001;
            }

            float velocity_x = rho_dot * cos(phi);
            float velocity_y = rho_dot * sin(phi);
            x << position_x, position_y, velocity_x , velocity_y;
        }
        
        // 避免运算时，0作为被除数
        if (fabs(x(0)) < 0.001) {
            x(0) = 0.001;
        }
        if (fabs(x(1)) < 0.001) {
            x(1) = 0.001;
        }
        // 初始化Kalman滤波器
        kf_.Initialization(x);

        // 设置协方差矩阵P
        Eigen::MatrixXd P = Eigen::MatrixXd(4, 4);
        P << 1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1000.0, 0.0,
             0.0, 0.0, 0.0, 1000.0;
        kf_.SetP(P);

        // 设置过程噪声Q
        Eigen::MatrixXd Q = Eigen::MatrixXd(4, 4);
        Q << 1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0;
        kf_.SetQ(Q);

        // 存储第一帧的时间戳，供下一帧数据使用
        last_timestamp_ = measurement_pack.timestamp_;
        is_initialized_ = true;//设置成True以后，后续的就执行非第一帧的情况了。
        return true;
    }

    /****** is_initialized_ = true;//设置成True以后，后续的就执行如下的情况*****/

    if (measurement_pack.timestamp_ < last_timestamp_) {
        RCLCPP_WARN(
          rclcpp::get_logger("sensor_fusion"),
          "Dropping out-of-order measurement: current=%" PRId64
          " ns, last=%" PRId64 " ns, lag=%.3f ms",
          static_cast<std::int64_t>(measurement_pack.timestamp_), last_timestamp_,
          (last_timestamp_ - measurement_pack.timestamp_) / 1000000.0);
        return false;
    }

    // 时间戳单位为纳秒，转换成秒。
    double delta_t = (measurement_pack.timestamp_ - last_timestamp_) / 1000000000.0; // unit : s
    RCLCPP_DEBUG(
      rclcpp::get_logger("sensor_fusion"),
      "last_timestamp_: %" PRId64 ", current_time: %" PRId64,
      last_timestamp_, static_cast<std::int64_t>(measurement_pack.timestamp_));
    last_timestamp_ = measurement_pack.timestamp_;

    // 同一时间戳的雷达和激光测量只做更新，避免重复注入过程噪声。
    if (delta_t > 0.0) {
        Eigen::MatrixXd F = Eigen::MatrixXd(4, 4);
        F << 1.0, 0.0, delta_t, 0.0,
             0.0, 1.0, 0.0, delta_t,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0;
        kf_.SetF(F);
        kf_.Prediction();
    }

    // 更新
    if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
        kf_.SetH(H_lidar_);     //代表测量矩阵
        kf_.SetR(R_lidar_);     //代表测量的方差矩阵
        kf_.KFUpdate(measurement_pack.raw_measurements_);
    } else if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
        kf_.SetR(R_radar_);
        // Jocobian矩阵Hj的运算已包含在EKFUpdate中
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
    kf_.Prediction();
    return true;
}

std::int64_t SensorFusion::LastTimestamp() const
{
    return last_timestamp_;
}
