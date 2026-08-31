#include <cstdint>

#include <gtest/gtest.h>

#include "sensorfusion.h"

namespace
{
MeasurementPackage lidarMeasurement(std::int64_t timestamp, double x, double y)
{
  MeasurementPackage measurement;
  measurement.sensor_type_ = MeasurementPackage::LASER;
  measurement.timestamp_ = timestamp;
  measurement.raw_measurements_ = Eigen::VectorXd(2);
  measurement.raw_measurements_ << x, y;
  return measurement;
}

MeasurementPackage radarMeasurement(
  std::int64_t timestamp, double rho, double phi, double rho_dot)
{
  MeasurementPackage measurement;
  measurement.sensor_type_ = MeasurementPackage::RADAR;
  measurement.timestamp_ = timestamp;
  measurement.raw_measurements_ = Eigen::VectorXd(3);
  measurement.raw_measurements_ << rho, phi, rho_dot;
  return measurement;
}

}  // namespace

TEST(SensorFusionTimestamp, RejectsOutOfOrderMeasurementWithoutMovingTimeBackward)
{
  SensorFusion fusion;
  ASSERT_TRUE(fusion.Process(lidarMeasurement(2'000'000'000LL, 1.0, 2.0)));

  EXPECT_FALSE(fusion.Process(lidarMeasurement(1'900'000'000LL, 1.1, 2.1)));
  EXPECT_EQ(fusion.LastTimestamp(), 2'000'000'000LL);

  EXPECT_TRUE(fusion.Process(lidarMeasurement(2'000'000'000LL, 1.2, 2.2)));
  EXPECT_EQ(fusion.LastTimestamp(), 2'000'000'000LL);

  EXPECT_TRUE(fusion.Process(lidarMeasurement(2'100'000'000LL, 1.3, 2.3)));
  EXPECT_EQ(fusion.LastTimestamp(), 2'100'000'000LL);
}

TEST(SensorFusionTimestamp, RejectsOutOfOrderPredictionWithoutThrowing)
{
  SensorFusion fusion;
  ASSERT_TRUE(fusion.Process(lidarMeasurement(2'000'000'000LL, 1.0, 2.0)));

  EXPECT_FALSE(fusion.Predict(1'900'000'000LL));
  EXPECT_EQ(fusion.LastTimestamp(), 2'000'000'000LL);

  EXPECT_TRUE(fusion.Predict(2'000'000'000LL));
  EXPECT_TRUE(fusion.Predict(2'100'000'000LL));
  EXPECT_EQ(fusion.LastTimestamp(), 2'100'000'000LL);
}

TEST(SensorFusionInitialization, PreservesNegativeRadarCoordinates)
{
  SensorFusion fusion;

  ASSERT_TRUE(
    fusion.Process(
      radarMeasurement(
        2'000'000'000LL,
        10.0,
        -0.2,
        2.0)));

  const Eigen::VectorXd state = fusion.kf_.GetX();

  EXPECT_NEAR(state(0), 9.8006658, 1e-5);
  EXPECT_NEAR(state(1), -1.9866933, 1e-5);
  EXPECT_NEAR(state(2), 1.9601332, 1e-5);
  EXPECT_NEAR(state(3), -0.3973387, 1e-5);
}
