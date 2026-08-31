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
