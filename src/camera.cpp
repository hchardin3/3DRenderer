#include "camera.hpp"

Camera::Camera(Eigen::Vector3d position, double horizontalFOV, double verticalFOV, 
                const int horizontalResolution, const int verticalResolution, double distance) :
    SceneObject(position, Eigen::Vector3d::UnitY(), Eigen::Vector3d::UnitZ()),
    m_horizontalFOV(horizontalFOV), 
    m_verticalFOV(verticalFOV),
    m_horizontalResolution(horizontalResolution),
    m_verticalResolution(verticalResolution),
    m_distance(distance),
    m_pixel_positions(Eigen::ArrayX3d::Zero(verticalResolution * horizontalResolution, 3)),
    m_rays(verticalResolution * horizontalResolution)
{
    m_horizontalRadPerPixel = m_horizontalFOV/m_horizontalResolution;
    m_verticalRadPerPixel = m_verticalFOV/m_verticalResolution;

    // Initialize the pixel positions matrix
    updatePixelPositions();
    updateRays();
}

void Camera::updatePixelPositions() {
    Eigen::Vector3d center_screen_position = (m_position + getForward() * m_distance);

    Eigen::ArrayXd Rows(m_verticalResolution * m_horizontalResolution), Cols(m_verticalResolution * m_horizontalResolution);
    Rows = Eigen::VectorXi::LinSpaced(m_verticalResolution, 0, m_verticalResolution - 1).replicate(1, m_horizontalResolution).reshaped(m_horizontalResolution * m_verticalResolution, 1).cast<double>();
    Cols = Eigen::VectorXi::LinSpaced(m_horizontalResolution, 0, m_horizontalResolution - 1).transpose().replicate(m_verticalResolution, 1).reshaped(m_horizontalResolution * m_verticalResolution, 1).cast<double>();

    m_pixel_positions = center_screen_position.transpose().replicate(m_verticalResolution * m_horizontalResolution, 1);

    Eigen::ArrayX3d upVectors = getUp().transpose().replicate(m_horizontalResolution * m_verticalResolution, 1);
    Eigen::ArrayX3d upCoordinates = ((-(Cols - m_horizontalResolution/2) * m_horizontalRadPerPixel).tan() * m_distance).replicate(1, 3);

    m_pixel_positions += upCoordinates * upVectors;


    Eigen::ArrayX3d rightVectors = getRight().transpose().replicate(m_horizontalResolution * m_verticalResolution, 1);
    Eigen::ArrayX3d rightCoordinates = (((Rows - m_verticalResolution/2) * m_verticalRadPerPixel).tan() * m_distance).replicate(1, 3);

    m_pixel_positions += rightCoordinates * rightVectors;

    // for(int i = 0; i < m_verticalResolution; ++i) {
    //     for(int j = 0; j < m_horizontalResolution; ++j) {
    //         pixelPosition = center_screen_position;

    //         pixelPosition += std::tan(-(i - m_horizontalResolution/2) * m_horizontalRadPerPixel) * m_distance * getUp();
    //         pixelPosition += std::tan((j - m_verticalResolution/2) * m_verticalRadPerPixel) * m_distance * getRight();

    //         // Store the pixel position in the matrix    
    //         m_pixel_positions(i, j) = getPositionPixel(i, j);
    //     }
    // }
}

void Camera::updateRays() {
    for(int i(0); i<m_verticalResolution; ++i) {
        for(int j(0); j<m_horizontalResolution; ++j) {
            m_rays[i * m_horizontalResolution + j].setOrigin(m_position);
            m_rays[i * m_horizontalResolution + j].setDirection(getPositionPixel(i, j) - m_position);
        }
    }
}

const std::tuple<const int, const int> Camera::getDimensions() const {
    return std::tuple<const int, const int>(m_verticalResolution, m_horizontalResolution);
}

Eigen::Vector3d Camera::getPositionPixel(const int i, const int j) const {
    return m_pixel_positions(i * m_horizontalResolution + j, Eigen::all);
}

const Ray& Camera::getRay(const int i, const int j) const {
    return m_rays[i * m_horizontalResolution + j];
}

const std::vector<Ray>& Camera::getRays() const {
    return m_rays;
}
