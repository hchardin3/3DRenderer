#pragma once

#include <Eigen/Dense>
#include <tuple>
#include "mesh.hpp"

struct Render
{
    typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, 3> MatrixXchar;
    MatrixXchar render;

    const int verticalResolution, horizontalResolution;

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
/// @param u The barycentric coordinate u of the intersection point (IP): IP = A + u * E1 + v * E2
/// @param v The barycentric coordinate v of the intersection point (IP): IP = A + u * E1 + v * E2
/// @param t The distance from the ray origin to the intersection point (IP): IP = R.Origin + t * R.Dir
/// @return true if the Ray intersect the object, false otherwise
bool intersect_triangle(Ray R, Triangle triangle, float& u, float& v, float& t);
