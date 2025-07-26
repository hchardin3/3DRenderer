#pragma once

#include <Eigen/Dense>
#include <tuple>
#include "sceneObject.hpp"
#include "Structures/ray.hpp"

class Camera : public SceneObject {
    private:
        /// @brief The horizontal field of view (in radian)
        double m_horizontalFOV;

        /// @brief The vertical field of view (in radian)
        double m_verticalFOV;

        /// @brief The horizontal resolution (in number of pixels)
        const unsigned int m_horizontalResolution;

        /// @brief The vertical resolution (in number of pixels)
        const unsigned int m_verticalResolution;

        /// @brief The horizontal field of view in radians per pixel
        double m_horizontalRadPerPixel;

        /// @brief The vertical field of view in radians per pixel
        double m_verticalRadPerPixel;

        /// @brief The distance between the eye and the projection plane (in meters)
        /// @note This is the distance from the camera to the projection plane where the pixels are
        double m_distance;


        /// @brief The pixel positions in the global frame
        /// @note This is an array of size (verticalResolution, horizontalResolution)
        Eigen::ArrayX3d m_pixel_positions;

        /// @brief A method to update the pixel positions in the global frame
        /// @note The pixel positions are updated based on the camera position and orientation
        void updatePixelPositions();

        /// @brief A vector of rays, each ray corresponds to a pixel in the frame
        /// @note The vector is of size (horizontalResolution * verticalResolution)
        std::vector<Ray> m_rays;

        /// @brief A method to update the rays that leave the camera and go through each pixel in the frame
        /// @note The rays are updated based on the camera position, orientation, and pixel positions
        void updateRays();

    public:
        /// @brief The camera is assumed to be initialized at the origin, facing the y-axis. 
        /// @param horizontalFOV The horizontal field of vue (in degree)
        /// @param verticalFOV The vertical field of vue (in degree)
        /// @param horizontalResolution The horizontal resolution (in number of pixels)
        /// @param verticalResolution The vertical field of vue (in number of pixels)
        /// @param distance The distance between the eye and the projection plane (in meters)
        Camera(Eigen::Vector3d position, double horizontalFOV, double verticalFOV,
                const unsigned int horizontalResolution, const unsigned int verticalResolution, double projectionDistance);


        /// @brief  A method to set the position of the camera in the global frame
        /// @param position The new position of the camera in the global frame
        /// @note The position is updated in the SceneObject class, and the pixel positions are updated accordingly
        void setPosition(const Eigen::Vector3d& position) {
            SceneObject::setPosition(position);
            updatePixelPositions();
            updateRays();
        };

        /// @brief A method to translate the camera along a displacement vector
        /// @param displacement The displacement vector
        void translate(const Eigen::Vector3d& displacement) {
            SceneObject::translate(displacement);
            updatePixelPositions();
            updateRays();
        };

        /// @brief A method to rotate the camera around a given axis
        /// @param axis The axis of rotation (must be a unit vector)
        void rotate(const Eigen::Vector3d& axis, const double angle) {
            SceneObject::rotate(axis, angle);
            updatePixelPositions();
            updateRays();
        };

        /// @brief A method to rotate the camera around a given rotation vector
        /// @param rotationVector The rotation vector (angle and axis of rotation)
        void rotate(const Eigen::Vector3d& rotationVector) {
            SceneObject::rotate(rotationVector);
            updatePixelPositions();
            updateRays();
        };

        /// @brief Return the number of pixels per lines and columns
        /// @return A tuple of two integers: the number of vertical and horizontal pixels
        const std::tuple<const int, const int> getDimensions() const;

        /// @brief A method to get the vector from the origin of the scene to the pixel (i, j)
        /// @param i The vertical index of the pixel in the frame
        /// @param j The horizontal index of the pixel in the frame
        /// @return The 3D position vector of the pixel in global frame
        Eigen::Vector3d getPositionPixel(const int i, const int j) const;

        /// @brief A method to get the light ray that leaves the origin of the camera and goes through the pixel (i, j)
        /// @param i The vertical index of the pixel in the frame
        /// @param j The horizontal index of the pixel in the frame
        /// @return The corresponding Ray (custom object)
        const Ray& getRay(const int i, const int j) const;

        /// @brief A method to get all the rays that leave the camera and go through each pixel in the frame
        /// @return A vector of rays, each ray corresponds to a pixel in the frame
        /// @note The vector is of size (horizontalResolution * verticalResolution)
        const std::vector<Ray>& getRays() const;
};
