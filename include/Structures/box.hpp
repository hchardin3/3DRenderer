#pragma once

#include <Eigen/Dense>
#include "Structures/ray.hpp"

// @brief A structure representing an Axis-Aligned Bounding Box (AABB) in 3D space.
// @details This structure is used to represent a 3D box defined by its minimum and maximum points in 3D space.
struct Box
{
    /// @brief The minimum points of the box
    Eigen::Array3d min;

    /// @brief The maximum points of the box
    Eigen::Array3d max;

    /// @brief A method to check if a point is inside the box
    /// @param point The point to check
    /// @return true if the point is inside the box, false otherwise
    bool contains(const Eigen::Array3d& point) const {
        return (point >= min && point <= max).all();
    };

    /// @brief A method to check if another box is contained within this box
    /// @param other The other box to check
    bool contains(const Box& other) const {
        return (other.min >= min && other.max <= max).all();
    };

    /// @brief A method to check if a ray intersects with this box
    /// @param ray The ray to check for intersection
    /// @param t The distance from the ray origin to the intersection point, only valid if the ray intersects the box
    /// @return true if the ray intersects the box, false otherwise
    bool intersect(const Ray& ray, double& t) const;
};
