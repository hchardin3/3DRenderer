#pragma once

#include <Eigen/Dense>

class Ray {
    private:
        Eigen::Vector3d m_origin;
        Eigen::Vector3d m_direction;

        Eigen::Vector3d m_inv; // Inverse of the direction vector for faster calculations

    public:
        /// @brief Constructor for the Ray class
        /// @param origin The origin of the ray (3-dim vector in meters)
        /// @param direction The direction of the ray (3-dim vector in meters)
        Ray(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction)
            : m_origin(origin), m_direction(direction.normalized()), m_inv(1.0 / direction.x(), 1.0 / direction.y(), 1.0 / direction.z()) {};

        /// @brief Default constructor for the Ray class
        /// @note Initializes the origin and direction to zero vectors and the inverse to infinity
        Ray() : m_origin(Eigen::Vector3d::Zero()), 
            m_direction(Eigen::Vector3d::Zero()), 
            m_inv(Eigen::Vector3d::Constant(std::numeric_limits<double>::infinity())) {};


        /// @brief Get the origin of the ray
        /// @return The origin of the ray as a 3-dim vector in meters
        inline const Eigen::Vector3d& getOrigin() const { return m_origin; };

        /// @brief Get the direction of the ray
        /// @return The direction of the ray as a 3-dim vector in meters
        inline const Eigen::Vector3d& getDirection() const { return m_direction; };

        /// @brief Get the inverse of the direction vector
        /// @return The inverse of the direction vector as a 3-dim vector
        inline const Eigen::Vector3d& getInverseDirection() const { return m_inv; };

        /// @brief Set the origin of the ray
        /// @param origin The new origin of the ray (3-dim vector in meters)
        void setOrigin(const Eigen::Vector3d& origin) { m_origin = origin; };

        /// @brief Set the direction of the ray
        /// @param direction The new direction of the ray (3-dim vector in meters)
        void setDirection(const Eigen::Vector3d& direction) {
            m_direction = direction.normalized();
            m_inv = Eigen::Vector3d(1.0 / m_direction.x(), 1.0 / m_direction.y(), 1.0 / m_direction.z());
        };
};
