#pragma once

#include <cstdint>

#include "interface/measurement_package.h"
#include "kalmanfilter.h"

class SensorFusion {
public:
    SensorFusion();
    ~SensorFusion();
    bool Predict(std::int64_t timestamp);
    bool Process(const MeasurementPackage & measurement_pack);
    std::int64_t LastTimestamp() const;
    KalmanFilter kf_;//实例化创建一个卡尔曼滤波的对象

private:
    bool is_initialized_;
    std::int64_t last_timestamp_;
    Eigen::MatrixXd R_lidar_;
    Eigen::MatrixXd R_radar_;
    Eigen::MatrixXd H_lidar_;
};
