#pragma once

#include <Eigen/Dense>

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
};
