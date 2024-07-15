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

        /// @brief Return the number of pixels per lines and columns
        /// @return A tuple of two integers: the number of vertical and horizontal pixels
        std::tuple<int, int> getDimensions() const;

        /// @brief A method to get the vector from the origin of the scene to the pixel (i, j)
        /// @param i The vertical index of the pixel in the frame
        /// @param j The horizontal index of the pixel in the frame
        /// @return The 3D position vector of the pixel in global frame
        Eigen::Vector3d getPositionPixel(const int i, const int j) const;

        /// @brief A method to get the light ray that leaves the origin of the camera and goes through the pixel (i, j)
        /// @param i The vertical index of the pixel in the frame
        /// @param j The horizontal index of the pixel in the frame
        /// @return The corresponding Ray (custom object)
        Ray getRay(const int i, const int j) const;

        /// @brief Move the camera to the given position. Has no effect on camera orientation.
        /// @param newPosition The new position where we want the camera (3D Vector)
        void moveCamera(Eigen::Vector3d newPosition);

        /// @brief Translate the camera along a displacement vector
        /// @param displacement The displacement vector
        void translateCamera(Eigen::Vector3d displacement);

        void rotateCamera(Eigen::Vector3d rotationVector);

};
