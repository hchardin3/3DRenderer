#pragma once

#include <Eigen/Dense>

struct Ray {
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};
