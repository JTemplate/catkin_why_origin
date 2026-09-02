#include <cmath>
#include <cstdint>

#include <gtest/gtest.h>

#include "sensorfusion.h"

namespace {

MeasurementPackage lidarMeasurement(
    std::int64_t timestamp,
    double x,
    double y)
{
    MeasurementPackage measurement;
    measurement.sensor_type_ = MeasurementPackage::LASER;
    measurement.raw_measurements_ = Eigen::VectorXd(2);
    measurement.raw_measurements_ << x, y;
    measurement.timestamp_ = timestamp;
    return measurement;
}

double positionError(
    const Eigen::VectorXd & state,
    double x,
    double y)
{
    return std::hypot(
        state(0) - x,
        state(1) - y);
}

}  // namespace


TEST(
    SensorFusionParameters,
    DefaultsPreserveCurrentFilterTuning)
{
    const SensorFusionConfig config;

    EXPECT_DOUBLE_EQ(
        config.lidar_measurement_variance_x,
        0.0225);

    EXPECT_DOUBLE_EQ(
        config.lidar_measurement_variance_y,
        0.0225);

    EXPECT_DOUBLE_EQ(
        config.radar_measurement_variance_range,
        0.09);

    EXPECT_DOUBLE_EQ(
        config.radar_measurement_variance_bearing,
        0.0009);

    EXPECT_DOUBLE_EQ(
        config.radar_measurement_variance_range_rate,
        0.09);

    EXPECT_DOUBLE_EQ(
        config.process_acceleration_variance_x,
        9.0);

    EXPECT_DOUBLE_EQ(
        config.process_acceleration_variance_y,
        9.0);
}


TEST(
    SensorFusionParameters,
    LidarMeasurementVarianceChangesCorrectionWeight)
{
    SensorFusionConfig low_noise_config;
    low_noise_config.lidar_measurement_variance_x =
        1.0e-6;

    SensorFusionConfig high_noise_config;
    high_noise_config.lidar_measurement_variance_x =
        1000.0;

    SensorFusion low_noise_fusion(
        low_noise_config);

    SensorFusion high_noise_fusion(
        high_noise_config);

    const auto initial =
        lidarMeasurement(
            1'000'000'000LL,
            1.0,
            1.0);

    ASSERT_TRUE(
        low_noise_fusion.Process(initial));

    ASSERT_TRUE(
        high_noise_fusion.Process(initial));

    // Same timestamp deliberately avoids prediction.
    // This isolates the LiDAR measurement covariance R.
    const auto correction =
        lidarMeasurement(
            1'000'000'000LL,
            2.0,
            1.0);

    ASSERT_TRUE(
        low_noise_fusion.Process(correction));

    ASSERT_TRUE(
        high_noise_fusion.Process(correction));

    const Eigen::VectorXd low_noise_state =
        low_noise_fusion.kf_.GetX();

    const Eigen::VectorXd high_noise_state =
        high_noise_fusion.kf_.GetX();

    // Lower measurement variance means the filter should trust
    // the LiDAR correction more strongly.
    EXPECT_LT(
        std::fabs(low_noise_state(0) - 2.0),
        std::fabs(high_noise_state(0) - 2.0));
}


TEST(
    SensorFusionParameters,
    RadarMeasurementVarianceChangesCorrectionWeight)
{
    SensorFusionConfig low_noise_config;
    low_noise_config.radar_measurement_variance_range =
        1.0e-6;
    low_noise_config.radar_measurement_variance_bearing =
        1.0e-6;
    low_noise_config.radar_measurement_variance_range_rate =
        1.0e-6;

    SensorFusionConfig high_noise_config;
    high_noise_config.radar_measurement_variance_range =
        1000.0;
    high_noise_config.radar_measurement_variance_bearing =
        1000.0;
    high_noise_config.radar_measurement_variance_range_rate =
        1000.0;

    SensorFusion low_noise_fusion(
        low_noise_config);

    SensorFusion high_noise_fusion(
        high_noise_config);

    const auto initial =
        lidarMeasurement(
            1'000'000'000LL,
            1.0,
            1.0);

    ASSERT_TRUE(
        low_noise_fusion.Process(initial));

    ASSERT_TRUE(
        high_noise_fusion.Process(initial));

    // Zero extrinsic is the default, so Cartesian (2, 1)
    // corresponds directly to the Radar target position here.
    ASSERT_TRUE(
        low_noise_fusion.ProcessRadarCartesian(
            2.0,
            1.0,
            0.0,
            1'000'000'000LL));

    ASSERT_TRUE(
        high_noise_fusion.ProcessRadarCartesian(
            2.0,
            1.0,
            0.0,
            1'000'000'000LL));

    const Eigen::VectorXd low_noise_state =
        low_noise_fusion.kf_.GetX();

    const Eigen::VectorXd high_noise_state =
        high_noise_fusion.kf_.GetX();

    EXPECT_LT(
        positionError(
            low_noise_state,
            2.0,
            1.0),
        positionError(
            high_noise_state,
            2.0,
            1.0));
}


TEST(
    SensorFusionParameters,
    AccelerationVarianceChangesPredictionUncertainty)
{
    SensorFusionConfig low_process_noise_config;
    low_process_noise_config.process_acceleration_variance_x =
        0.0;
    low_process_noise_config.process_acceleration_variance_y =
        0.0;

    SensorFusionConfig high_process_noise_config;
    high_process_noise_config.process_acceleration_variance_x =
        100.0;
    high_process_noise_config.process_acceleration_variance_y =
        100.0;

    SensorFusion low_process_noise_fusion(
        low_process_noise_config);

    SensorFusion high_process_noise_fusion(
        high_process_noise_config);

    // Repeated stationary observations first reduce uncertainty
    // inherited from the deliberately large initial velocity P.
    for (std::int64_t second = 1;
         second <= 3;
         ++second)
    {
        const auto stationary =
            lidarMeasurement(
                second * 1'000'000'000LL,
                1.0,
                1.0);

        ASSERT_TRUE(
            low_process_noise_fusion.Process(
                stationary));

        ASSERT_TRUE(
            high_process_noise_fusion.Process(
                stationary));
    }

    const auto moved =
        lidarMeasurement(
            4'000'000'000LL,
            2.0,
            1.0);

    ASSERT_TRUE(
        low_process_noise_fusion.Process(moved));

    ASSERT_TRUE(
        high_process_noise_fusion.Process(moved));

    const Eigen::VectorXd low_process_noise_state =
        low_process_noise_fusion.kf_.GetX();

    const Eigen::VectorXd high_process_noise_state =
        high_process_noise_fusion.kf_.GetX();

    // Larger acceleration variance creates more prediction
    // uncertainty, so the next measurement should receive
    // greater correction weight.
    EXPECT_LT(
        std::fabs(
            high_process_noise_state(0) - 2.0),
        std::fabs(
            low_process_noise_state(0) - 2.0));
}
