#pragma once

#include <Eigen/Dense>
#include <tuple>
#include "mesh.hpp"

struct Render
{
    typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> MatrixXchar;
    MatrixXchar render;

    /// @brief The image frame to be printed on the user's screen
    /// @param verticalResolution 
    /// @param horizontalResolution 
    Render(int verticalResolution, int horizontalResolution);

    Render(std::tuple<int, int> resolution);
};

struct Ray {
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};


/// @brief Return true if the ray intersects the triangle
/// @param R The Ray
/// @param triangle The triangle
/// @param t The distance from the ray origin to the intersection point (IP): IP = R.Origin + t * R.Dir
/// @return true if the Ray intersect the object, false otherwise
bool intersect_triangle(Ray R, Triangle triangle, float& t);
