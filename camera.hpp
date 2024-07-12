#pragma once

#include <eigen3/Eigen/Dense>
#include <tuple>
#include "utils.hpp"

class Camera {
    private:
        Eigen::Vector3d m_position;
        Eigen::Vector3d m_up;
        Eigen::Vector3d m_forward;
        Eigen::Vector3d m_right;

        double m_horizontalFOV;
        double m_verticalFOV;
        int m_horizontalResolution;
        int m_verticalResolution;
        double m_horizontalRadPerPixel;
        double m_verticalRadPerPixel;
        double m_distance;

        Eigen::MatrixXd m_renderer;


    public:
        /// @brief The camera is assumed to be initialized at the origin, facing the y-axis. 
        /// @param horizontalFOV The horizontal field of vue (in degree)
        /// @param verticalFOV The vertical field of vue (in degree)
        /// @param horizontalResolution The horizontal resolution (in number of pixels)
        /// @param verticalResolution The vertical field of vue (in number of pixels)
        /// @param distance The distance between the eye and the projection plane (in meters)
        Camera(double horizontalFOV, double verticalFOV,  int horizontalResolution,  int verticalResolution,  double distance);

        std::tuple<int, int> getDimensions() const;

        Eigen::Vector3d getPositionPixel(const int i, const int j) const;

        Ray getRay(const int i, const int j) const;

};
