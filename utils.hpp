#pragma once

#include <eigen3/Eigen/Dense>
#include <tuple>
#include "mesh.hpp"



struct Render
{
    typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> MatrixXchar;
    MatrixXchar render;

    Render(int verticalResolution, int horizontalResolution);

    Render(std::tuple<int, int> resolution);
};

struct Ray {
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};


/// @brief Return true if the ray intersects the triangle
/// @param R 
/// @param A 
/// @param B 
/// @param C 
/// @return 
bool intersect_triangle(Ray R, Triangle triangle, float& t);