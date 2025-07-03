#pragma once

#include <Eigen/Dense>
#include <tuple>
#include "utils.hpp"
#include "sceneObject.hpp"

class Camera : public SceneObject {
    private:
        /// @brief The horizontal field of view (in radian)
        double m_horizontalFOV;

        /// @brief The vertical field of view (in radian)
        double m_verticalFOV;

        /// @brief The horizontal resolution (in number of pixels)
        int m_horizontalResolution;

        /// @brief The vertical resolution (in number of pixels)
        int m_verticalResolution;

        /// @brief The horizontal field of view in radians per pixel
        double m_horizontalRadPerPixel;

        /// @brief The vertical field of view in radians per pixel
        double m_verticalRadPerPixel;

        /// @brief The distance between the eye and the projection plane (in meters)
        /// @note This is the distance from the camera to the projection plane where the pixels are
        double m_distance;

        /// @brief The renderer matrix that will hold the rendered image
        /// @note This is a matrix of size (horizontalResolution, verticalResolution)
        /// @note The matrix is initialized to zero and will be filled with pixel values during rendering
        Eigen::MatrixXd m_renderer;

    public:
        /// @brief The camera is assumed to be initialized at the origin, facing the y-axis. 
        /// @param horizontalFOV The horizontal field of vue (in degree)
        /// @param verticalFOV The vertical field of vue (in degree)
        /// @param horizontalResolution The horizontal resolution (in number of pixels)
        /// @param verticalResolution The vertical field of vue (in number of pixels)
        /// @param distance The distance between the eye and the projection plane (in meters)
        Camera(Eigen::Vector3d position, double horizontalFOV, double verticalFOV,  int horizontalResolution,  int verticalResolution,  double distance);

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
};
