#pragma once

#include <Eigen/Dense>
#include "sceneObject.hpp"

class LightSource : public SceneObject {
    private:
        /// @brief The color of the light source
        Eigen::Vector3d m_color;

        /// @brief The intensity of the light source
        unsigned char m_intensity;

    public:
        /// @brief A simple light source in the scene
        /// @param position The position of the light source in the global frame
        /// @param color The color of the light source (RGB)
        /// @param intensity The intensity of the light source
        LightSource(const Eigen::Vector3d& position, const Eigen::Vector3d& color, unsigned char intensity);

        /// @brief Get the color of the light source
        /// @return The color of the light source (RGB)
        const Eigen::Vector3d& getColor() const { return m_color; };

        /// @brief Get the intensity of the light source
        /// @return The intensity of the light source
        unsigned char getIntensity() const { return m_intensity; };

};