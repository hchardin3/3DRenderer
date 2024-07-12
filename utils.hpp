#pragma once

#include <eigen3/Eigen/Dense>
#include <tuple>



struct Render
{
    typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> MatrixXchar;
    MatrixXchar render;

    Render(int verticalResolution, int horizontalResolution);

    Render(std::tuple<int, int> resolution);
};
