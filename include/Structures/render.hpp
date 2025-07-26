#pragma once

#include <Eigen/Dense>
#include <tuple>

struct Render
{
    typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, 3> MatrixXchar;
    MatrixXchar render;

    const unsigned int verticalResolution, horizontalResolution;

    /// @brief The image frame to be printed on the user's screen
    /// @param verticalResolution 
    /// @param horizontalResolution 
    Render(unsigned int verticalResolution, unsigned int horizontalResolution) :
        render(MatrixXchar::Zero(verticalResolution * horizontalResolution, 3)),
        verticalResolution(verticalResolution), horizontalResolution(horizontalResolution) {
    }

    /// @brief The image frame to be printed on the user's screen
    /// @param resolution A tuple containing the vertical and horizontal resolutions
    /// @note The resolution is given as a tuple of (verticalResolution, horizontalResolution)
    Render(std::tuple<unsigned int, unsigned int> resolution) :
        render(MatrixXchar::Zero(std::get<0>(resolution) * std::get<1>(resolution), 3)),
        verticalResolution(std::get<0>(resolution)), horizontalResolution(std::get<1>(resolution)) {
    }
};