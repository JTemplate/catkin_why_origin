#pragma once

#include <cstdint>

#include "interface/measurement_package.h"
#include "kalmanfilter.h"

bool ShouldProcessLidarFirst(
    std::int64_t lidar_timestamp,
    std::int64_t radar_timestamp);

struct SensorFusionConfig {
    double lidar_measurement_variance_x = 0.0225;
    double lidar_measurement_variance_y = 0.0225;

    double radar_measurement_variance_range = 0.09;
    double radar_measurement_variance_bearing = 0.0009;
    double radar_measurement_variance_range_rate = 0.09;

    double process_acceleration_variance_x = 9.0;
    double process_acceleration_variance_y = 9.0;
};


class SensorFusion {
public:
    SensorFusion();
    ~SensorFusion();

    void SetRadarExtrinsic(
        double translation_x,
        double translation_y,
        double yaw);

    bool Predict(std::int64_t timestamp);

    bool Process(
        const MeasurementPackage & measurement_pack);

    bool ProcessRadarCartesian(
        double x_velodyne,
        double y_velodyne,
        double radial_velocity,
        std::int64_t timestamp);

    std::int64_t LastTimestamp() const;

    KalmanFilter kf_;


    explicit SensorFusion(
    const SensorFusionConfig & config);


private:
    bool is_initialized_;
    std::int64_t last_timestamp_;

    double radar_translation_x_ = 0.0;
    double radar_translation_y_ = 0.0;
    double radar_yaw_ = 0.0;

    Eigen::MatrixXd R_lidar_;
    Eigen::MatrixXd R_radar_;
    Eigen::MatrixXd H_lidar_;

    double process_acceleration_variance_x_ = 9.0;
    double process_acceleration_variance_y_ = 9.0;

};
