#pragma once

#include <Eigen/Dense>
#include <tuple>
#include "mesh.hpp"
#include "Structures/ray.hpp"


/// @brief Return true if the ray intersects the triangle
/// @param R The Ray
/// @param triangle The triangle
/// @param u The barycentric coordinate u of the intersection point (IP): IP = A + u * E1 + v * E2
/// @param v The barycentric coordinate v of the intersection point (IP): IP = A + u * E1 + v * E2
/// @param t The distance from the ray origin to the intersection point (IP): IP = R.Origin + t * R.Dir
/// @return true if the Ray intersect the object, false otherwise
bool intersect_triangle(Ray R, Triangle triangle, float& u, float& v, float& t);
