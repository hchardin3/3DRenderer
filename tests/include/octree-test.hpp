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
};
