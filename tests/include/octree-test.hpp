#pragma once
#include <doctest/doctest.h>

#include "Structures/octree.hpp"
#include "triangle.hpp"

class MockTriangle {
    public:
        Eigen::Vector3d position;

        MockTriangle(const Eigen::Vector3d& pos) : position(pos) {}

        Eigen::Vector3d getPosition() const {
            return position;
        }

        bool intersect(const Ray& ray, float& u, float& v, float& t) const {
            // Mock intersection logic
            // For testing purposes, we can assume it always intersects
            u = 0.5f; v = 0.5f; t = ray.getOrigin().norm(); // Just a mock value
            return true;
        }
};
