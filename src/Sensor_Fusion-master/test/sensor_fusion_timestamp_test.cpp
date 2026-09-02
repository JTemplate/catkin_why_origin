#include <cstdint>

#include <cmath>

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

TEST(KalmanFilterEkf, HandlesNearOriginStateWithoutProducingInvalidValues)
{
  KalmanFilter filter;

  Eigen::VectorXd initial_state(4);
  initial_state << 0.0, 0.0, 1.0, -1.0;
  filter.Initialization(initial_state);

  filter.SetP(Eigen::MatrixXd::Identity(4, 4));
  filter.SetR(Eigen::MatrixXd::Identity(3, 3));

  Eigen::VectorXd radar_measurement(3);
  radar_measurement << 1.0, 0.2, 0.0;

  filter.EKFUpdate(radar_measurement);

  const Eigen::VectorXd state = filter.GetX();

  ASSERT_EQ(state.size(), 4);

  for (int i = 0; i < state.size(); ++i) {
    EXPECT_TRUE(std::isfinite(state(i)));
  }

  EXPECT_NEAR(state(0), 0.0, 1e-12);
  EXPECT_NEAR(state(1), 0.0, 1e-12);
  EXPECT_NEAR(state(2), 1.0, 1e-12);
  EXPECT_NEAR(state(3), -1.0, 1e-12);
}

TEST(KalmanFilterEkf, HandlesRadarAngleAcrossPiBoundary)
{
  KalmanFilter filter;

  Eigen::VectorXd initial_state(4);
  initial_state << -1.0, 0.01, 0.0, 0.0;
  filter.Initialization(initial_state);

  filter.SetP(Eigen::MatrixXd::Identity(4, 4));
  filter.SetR(Eigen::MatrixXd::Identity(3, 3));

  const double pi = std::acos(-1.0);

  Eigen::VectorXd radar_measurement(3);
  radar_measurement <<
    std::sqrt(1.0001),
    -pi + 0.01,
    0.0;

  filter.EKFUpdate(radar_measurement);

  const Eigen::VectorXd state = filter.GetX();

  ASSERT_EQ(state.size(), 4);

  for (int i = 0; i < state.size(); ++i) {
    EXPECT_TRUE(std::isfinite(state(i)));
  }

  EXPECT_LT(std::fabs(state(1)), 0.1);
  EXPECT_LT(state(0), 0.0);
}

TEST(SensorFusionTimestamp, OrdersSynchronizedMeasurementsChronologically)
{
  EXPECT_TRUE(
    ShouldProcessLidarFirst(
      1'000'000'000LL,
      1'030'000'000LL));

  EXPECT_FALSE(
    ShouldProcessLidarFirst(
      1'030'000'000LL,
      1'000'000'000LL));

  EXPECT_TRUE(
    ShouldProcessLidarFirst(
      1'000'000'000LL,
      1'000'000'000LL));
}

TEST(KalmanFilterEkf, UsesRadarExtrinsicOriginInMeasurementModel)
{
  KalmanFilter filter;

  Eigen::VectorXd initial_state(4);
  initial_state << 1.26, 0.06, 1.0, 0.0;
  filter.Initialization(initial_state);

  filter.SetP(Eigen::MatrixXd::Identity(4, 4));
  filter.SetR(Eigen::MatrixXd::Identity(3, 3));

  // Radar origin is at (0.26, 0.06) in the velodyne frame.
  filter.SetRadarExtrinsic(0.26, 0.06, 0.0);

  // Relative to the Radar origin, the target is:
  // position = (1.0, 0.0)
  // velocity = (1.0, 0.0)
  //
  // Therefore the ideal Radar measurement is:
  // rho = 1, phi = 0, rho_dot = 1.
  Eigen::VectorXd radar_measurement(3);
  radar_measurement << 1.0, 0.0, 1.0;

  filter.EKFUpdate(radar_measurement);

  const Eigen::VectorXd state = filter.GetX();

  ASSERT_EQ(state.size(), 4);

  EXPECT_NEAR(state(0), 1.26, 1e-9);
  EXPECT_NEAR(state(1), 0.06, 1e-9);
  EXPECT_NEAR(state(2), 1.0, 1e-9);
  EXPECT_NEAR(state(3), 0.0, 1e-9);
}

TEST(KalmanFilterEkf, UsesRadarExtrinsicYawInMeasurementModel)
{
  KalmanFilter filter;

  const double pi = std::acos(-1.0);

  // Radar frame is translated to (0.26, 0.06)
  // and rotated +90 degrees in the velodyne frame.
  filter.SetRadarExtrinsic(
    0.26,
    0.06,
    pi / 2.0);

  // In the Radar frame, target position and velocity are:
  // position = (1, 0)
  // velocity = (1, 0)
  //
  // With Radar yaw = +90 deg, this becomes in velodyne:
  // position = (0.26, 1.06)
  // velocity = (0, 1)
  Eigen::VectorXd initial_state(4);
  initial_state << 0.26, 1.06, 0.0, 1.0;
  filter.Initialization(initial_state);

  filter.SetP(Eigen::MatrixXd::Identity(4, 4));
  filter.SetR(Eigen::MatrixXd::Identity(3, 3));

  Eigen::VectorXd radar_measurement(3);
  radar_measurement << 1.0, 0.0, 1.0;

  filter.EKFUpdate(radar_measurement);

  const Eigen::VectorXd state = filter.GetX();

  ASSERT_EQ(state.size(), 4);

  EXPECT_NEAR(state(0), 0.26, 1e-9);
  EXPECT_NEAR(state(1), 1.06, 1e-9);
  EXPECT_NEAR(state(2), 0.0, 1e-9);
  EXPECT_NEAR(state(3), 1.0, 1e-9);
}

TEST(SensorFusionInitialization, AppliesRadarExtrinsicToInitialState)
{
  SensorFusion fusion;

  const double pi = std::acos(-1.0);

  fusion.SetRadarExtrinsic(
    0.26,
    0.06,
    pi / 2.0);

  // In the Radar frame:
  // position = (1, 0)
  // velocity = (1, 0)
  //
  // With Radar extrinsic:
  // translation = (0.26, 0.06)
  // yaw = +90 deg
  //
  // Expected velodyne-frame state:
  // position = (0.26, 1.06)
  // velocity = (0, 1)
  ASSERT_TRUE(
    fusion.Process(
      radarMeasurement(
        2'000'000'000LL,
        1.0,
        0.0,
        1.0)));

  const Eigen::VectorXd state = fusion.kf_.GetX();

  ASSERT_EQ(state.size(), 4);

  EXPECT_NEAR(state(0), 0.26, 1e-9);
  EXPECT_NEAR(state(1), 1.06, 1e-9);
  EXPECT_NEAR(state(2), 0.0, 1e-9);
  EXPECT_NEAR(state(3), 1.0, 1e-9);
}

TEST(
    SensorFusionInitialization,
    ProcessRadarCartesianUsesRadarExtrinsic)
{
    SensorFusion fusion;

    const double pi = std::acos(-1.0);

    // Radar origin in velodyne:
    // translation = (0.26, 0.06)
    // yaw = +90 degrees
    fusion.SetRadarExtrinsic(
        0.26,
        0.06,
        pi / 2.0);

    // Choose a target that is at (1, 0) in the Radar frame.
    //
    // After applying the Radar extrinsic, its velodyne position is:
    //
    // x = 0.26
    // y = 1.06
    //
    // radial_velocity = 2.5 m/s along the Radar +X direction.
    //
    // After rotating into velodyne, expected velocity is:
    //
    // vx = 0
    // vy = 2.5
    ASSERT_TRUE(
        fusion.ProcessRadarCartesian(
            0.26,
            1.06,
            2.5,
            2'000'000'000LL));

    const Eigen::VectorXd state = fusion.kf_.GetX();

    ASSERT_EQ(state.size(), 4);

    EXPECT_NEAR(state(0), 0.26, 1e-9);
    EXPECT_NEAR(state(1), 1.06, 1e-9);
    EXPECT_NEAR(state(2), 0.0, 1e-9);
    EXPECT_NEAR(state(3), 2.5, 1e-9);
}

TEST(SensorFusionProcessNoise, ScalesWithPredictionInterval)
{
  constexpr std::int64_t initial_timestamp =
    2'000'000'000LL;

  SensorFusion short_step;
  SensorFusion long_step;

  ASSERT_TRUE(
    short_step.Process(
      lidarMeasurement(
        initial_timestamp,
        10.0,
        5.0)));

  ASSERT_TRUE(
    long_step.Process(
      lidarMeasurement(
        initial_timestamp,
        10.0,
        5.0)));

  // Compare dt = 0.1 s against dt = 0.2 s.
  ASSERT_TRUE(
    short_step.Predict(
      initial_timestamp +
      100'000'000LL));

  ASSERT_TRUE(
    long_step.Predict(
      initial_timestamp +
      200'000'000LL));

  const Eigen::MatrixXd q_short =
    short_step.kf_.GetQ();

  const Eigen::MatrixXd q_long =
    long_step.kf_.GetQ();

  ASSERT_EQ(q_short.rows(), 4);
  ASSERT_EQ(q_short.cols(), 4);
  ASSERT_EQ(q_long.rows(), 4);
  ASSERT_EQ(q_long.cols(), 4);

  // A white-acceleration constant-velocity model has:
  //
  // position variance      ~ dt^4
  // position/velocity term ~ dt^3
  // velocity variance      ~ dt^2
  //
  // Doubling dt therefore gives factors 16, 8 and 4.
  EXPECT_GT(q_short(0, 0), 0.0);
  EXPECT_GT(q_short(0, 2), 0.0);
  EXPECT_GT(q_short(2, 2), 0.0);

  EXPECT_NEAR(
    q_long(0, 0),
    16.0 * q_short(0, 0),
    1e-9);

  EXPECT_NEAR(
    q_long(0, 2),
    8.0 * q_short(0, 2),
    1e-9);

  EXPECT_NEAR(
    q_long(2, 2),
    4.0 * q_short(2, 2),
    1e-9);

  EXPECT_NEAR(
    q_long(1, 1),
    16.0 * q_short(1, 1),
    1e-9);

  EXPECT_NEAR(
    q_long(1, 3),
    8.0 * q_short(1, 3),
    1e-9);

  EXPECT_NEAR(
    q_long(3, 3),
    4.0 * q_short(3, 3),
    1e-9);
}

TEST(SensorFusionProcessNoise, ProcessAndPredictUseSameProcessNoise)
{
  constexpr std::int64_t initial_timestamp =
    2'000'000'000LL;

  constexpr std::int64_t next_timestamp =
    initial_timestamp +
    100'000'000LL;

  SensorFusion process_path;
  SensorFusion predict_path;

  ASSERT_TRUE(
    process_path.Process(
      lidarMeasurement(
        initial_timestamp,
        10.0,
        5.0)));

  ASSERT_TRUE(
    predict_path.Process(
      lidarMeasurement(
        initial_timestamp,
        10.0,
        5.0)));

  ASSERT_TRUE(
    process_path.Process(
      lidarMeasurement(
        next_timestamp,
        10.1,
        5.0)));

  ASSERT_TRUE(
    predict_path.Predict(
      next_timestamp));

  const Eigen::MatrixXd process_q =
    process_path.kf_.GetQ();

  const Eigen::MatrixXd predict_q =
    predict_path.kf_.GetQ();

  ASSERT_EQ(process_q.rows(), predict_q.rows());
  ASSERT_EQ(process_q.cols(), predict_q.cols());

  EXPECT_TRUE(
    process_q.isApprox(
      predict_q,
      1e-12));
}
