#include "camera.hpp"

Camera::Camera(double horizontalFOV, double verticalFOV,  int horizontalResolution,  int verticalResolution,  double distance) :
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
        m_right = m_forward.cross(m_up);

        m_renderer = Eigen::MatrixXd::Zero(horizontalResolution, verticalResolution);

        m_horizontalRadPerPixel = m_horizontalFOV/m_horizontalResolution;
        m_verticalRadPerPixel = m_verticalFOV/m_verticalResolution;
    }

std::tuple<int, int> Camera::getDimensions() const {
    return std::tuple<int, int>(m_verticalResolution, m_horizontalResolution);
}

Eigen::Vector3d Camera::getPositionPixel(const int i, const int j) const {
    Eigen::Vector3d pixelPosition = m_position + m_forward * m_distance;

    pixelPosition += std::tan(-(i - m_horizontalResolution/2) * m_horizontalRadPerPixel) * m_distance * m_up;
    pixelPosition += std::tan((j - m_verticalResolution/2) * m_verticalRadPerPixel) * m_distance * m_right;

    return pixelPosition;
}

Ray Camera::getRay(const int i, const int j) const {
    Ray ray;
    ray.origin = m_position;
    Eigen::Vector3d pixel_pos = getPositionPixel(i, j);
    ray.direction = (pixel_pos - m_position).normalized();
    return ray;
}