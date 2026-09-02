/*****
 *HITWH606
 *Jack Ju
 *kalman filter 
 ******/ 
/***
 * 程序说明：
 * 本程序是对kalmanfilter.h中的函数具体的实现
 * **/
#include "kalmanfilter.h"
#include "iostream"
#include <cmath>

using namespace std;
KalmanFilter::KalmanFilter()
{
    is_initialized_ = false;
}

KalmanFilter::~KalmanFilter()
{

}

void KalmanFilter::Initialization(Eigen::VectorXd x_in)
{
    x_ = x_in;
}

bool KalmanFilter::IsInitialized()
{
    return is_initialized_;
}

void KalmanFilter::SetF(Eigen::MatrixXd F_in)
{
    F_ = F_in;
}

void KalmanFilter::SetP(Eigen::MatrixXd P_in)
{
    P_ = P_in;
}

void KalmanFilter::SetQ(Eigen::MatrixXd Q_in)
{
    Q_ = Q_in;
}

void KalmanFilter::SetH(Eigen::MatrixXd H_in)
{
    H_ = H_in;
}

void KalmanFilter::SetR(Eigen::MatrixXd R_in)
{
    R_ = R_in;
}

void KalmanFilter::Prediction()
{
    x_ = F_ * x_;//模型可修改
    Eigen::MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;
    //cout<<P_<<endl;
}
void KalmanFilter::SetRadarExtrinsic(
    double translation_x,
    double translation_y,
    double yaw)
{
    radar_translation_x_ = translation_x;
    radar_translation_y_ = translation_y;
    radar_yaw_ = yaw;
}

void KalmanFilter::KFUpdate(Eigen::VectorXd z)
{
    Eigen::VectorXd y = z - H_ * x_;
    Eigen::MatrixXd Ht = H_.transpose();
    Eigen::MatrixXd S = H_ * P_ * Ht + R_;
    Eigen::MatrixXd Si = S.inverse();
    Eigen::MatrixXd K =  P_ * Ht * Si;
    x_ = x_ + (K * y);
    int x_size = x_.size();
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(x_size, x_size);//单位阵
    P_ = (I - K * H_) * P_; 
}

void KalmanFilter::EKFUpdate(Eigen::VectorXd z)
{
    if (!CalculateJacobianMatrix()) {
        return;
    }

    const double cos_yaw = std::cos(radar_yaw_);
    const double sin_yaw = std::sin(radar_yaw_);

    // Transform the state from the velodyne frame into the Radar frame.
    const double dx = x_(0) - radar_translation_x_;
    const double dy = x_(1) - radar_translation_y_;

    const double px_r = cos_yaw * dx + sin_yaw * dy;
    const double py_r = -sin_yaw * dx + cos_yaw * dy;

    const double vx_r =
        cos_yaw * x_(2) + sin_yaw * x_(3);
    const double vy_r =
        -sin_yaw * x_(2) + cos_yaw * x_(3);

    const double rho =
        std::sqrt(px_r * px_r + py_r * py_r);
    const double theta = std::atan2(py_r, px_r);
    const double rho_dot =
        (px_r * vx_r + py_r * vy_r) / rho;

    Eigen::VectorXd h(3);
    h << rho, theta, rho_dot;

    Eigen::VectorXd y = z - h;

    y(1) = std::atan2(
        std::sin(y(1)),
        std::cos(y(1)));

    Eigen::MatrixXd Ht = H_.transpose();
    Eigen::MatrixXd S = H_ * P_ * Ht + R_;
    Eigen::MatrixXd Si = S.inverse();
    Eigen::MatrixXd K = P_ * Ht * Si;

    x_ = x_ + K * y;

    int x_size = x_.size();
    Eigen::MatrixXd I =
        Eigen::MatrixXd::Identity(x_size, x_size);

    P_ = (I - K * H_) * P_;
}



Eigen::VectorXd KalmanFilter::GetX()
{
    return x_;
}

Eigen::MatrixXd KalmanFilter::GetQ() const
{
    return Q_;
}

bool KalmanFilter::CalculateJacobianMatrix()
{
    const double cos_yaw = std::cos(radar_yaw_);
    const double sin_yaw = std::sin(radar_yaw_);

    const double dx = x_(0) - radar_translation_x_;
    const double dy = x_(1) - radar_translation_y_;

    const double px_r = cos_yaw * dx + sin_yaw * dy;
    const double py_r = -sin_yaw * dx + cos_yaw * dy;

    const double vx_r =
        cos_yaw * x_(2) + sin_yaw * x_(3);
    const double vy_r =
        -sin_yaw * x_(2) + cos_yaw * x_(3);

    const double c1 = px_r * px_r + py_r * py_r;

    if (c1 < 0.0001) {
        return false;
    }

    const double c2 = std::sqrt(c1);
    const double c3 = c1 * c2;

    // Standard Radar Jacobian in the Radar coordinate frame.
    Eigen::MatrixXd Hj_radar(3, 4);

    Hj_radar <<
        px_r / c2,
        py_r / c2,
        0,
        0,

        -py_r / c1,
        px_r / c1,
        0,
        0,

        py_r * (vx_r * py_r - vy_r * px_r) / c3,
        px_r * (px_r * vy_r - py_r * vx_r) / c3,
        px_r / c2,
        py_r / c2;

    // Jacobian of the velodyne-frame state transformed into
    // the Radar coordinate frame.
    Eigen::MatrixXd radar_from_velodyne =
        Eigen::MatrixXd::Zero(4, 4);

    radar_from_velodyne <<
        cos_yaw,  sin_yaw, 0,        0,
        -sin_yaw, cos_yaw, 0,        0,
        0,        0,        cos_yaw,  sin_yaw,
        0,        0,       -sin_yaw, cos_yaw;

    H_ = Hj_radar * radar_from_velodyne;

    return true;
}
