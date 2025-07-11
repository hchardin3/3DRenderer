#pragma once

#include <Eigen/Dense>
#include "Structures/ray.hpp"

struct Plane {
    /// @brief The normal vector of the plane
    Eigen::Vector3d normal;

    /// @brief A point on the plane
    Eigen::Vector3d point;

    /// @brief Constructor for the Plane class
    /// @param normal The normal vector of the plane (3-dim vector)
    /// @param point A point on the plane (3-dim vector)
    /// @note The normal vector is normalized in the constructor
    Plane(const Eigen::Vector3d& normal, const Eigen::Vector3d& point)
        : normal(normal.normalized()), point(point), m_point_dot_normal(point.dot(normal)) {
            // Ensure that the normal vector is non zero
            if (normal.norm() < std::numeric_limits<double>::epsilon()) {
                throw std::invalid_argument("Normal vector cannot be zero.");
            }
        }

    /// @brief A branchless method to check if a ray intersects with the plane
    /// @param ray The ray to check for intersection
    /// @param t The distance from the ray origin to the intersection point, only valid if the ray intersects the plane
    /// @return true if the ray intersects the plane, false otherwise
    /// @note The method calculates the intersection point using the formula from 
    ///     https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
    bool intersect(const Ray& ray, double& t) const {
        double denominator = normal.dot(ray.getDirection());

        double denom_valid = std::fabs(denominator) >= std::numeric_limits<double>::epsilon();
        double safe_denominator = denominator + (1.0 - denom_valid); // Add 1 if denominator is zero

        // Calculate the distance from the ray origin to the intersection point
        t = (m_point_dot_normal - ray.getOrigin().dot(normal)) / safe_denominator;

        // Check if the intersection point is in front of the ray origin
        return denom_valid && t >= 0;
    }

    /// @brief Get the dot product of the point on the plane and the normal vector
    /// @return The dot product of the point on the plane and the normal vector
    double getPointDotNormal() const {
        return m_point_dot_normal;
    }

    private:
        double m_point_dot_normal;
};