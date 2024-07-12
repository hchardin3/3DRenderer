#include "camera.hpp"

Camera::Camera(double horizontalFOV, double verticalFOV,  double horizontalResolution,  double verticalResolution,  double distance) :
    m_horizontalFOV(horizontalFOV), 
    m_verticalFOV(verticalFOV),
    m_horizontalResolution(horizontalResolution),
    m_verticalResolution(verticalResolution),
    m_distance(distance) 
    {
        m_position = Eigen::Vector3d::Zero();
        m_up = Eigen::Vector3d::Zero();
        m_forward = Eigen::Vector3d::Zero();

        m_up.z() = 1.0;
        m_forward.y() = 1.0;

        m_renderer = Eigen::MatrixXd::Zero(horizontalResolution, verticalResolution);
    }