#include "light.hpp"

LightSource::LightSource(const Eigen::Vector3d& position, const Eigen::Vector3d& color, unsigned char intensity) :
    SceneObject(position, Eigen::Vector3d::UnitY(), Eigen::Vector3d::UnitZ()),
    m_color(color),
    m_intensity(intensity) 
{
    if (m_color.size() != 3) {
        throw std::invalid_argument("Color must be a 3D vector (RGB).");
    }
}
