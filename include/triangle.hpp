#pragma once

#include <Eigen/Dense>
#include <tuple>
#include <csignal>
#include "sceneObject.hpp"
#include "Structures/ray.hpp"
#include "Structures/box.hpp"

class Triangle : public SceneObject {    
    public:
        /// @brief A very simple triangular mesh
        /// @param point0 The coordinates of the first point of the mesh (3-dim vector in meters)
        /// @param point1 The coordinates of the second point of the mesh (3-dim vector in meters)
        /// @param point2 The coordinates of the third point of the mesh (3-dim vector in meters)
        /// @param invert Wether the normal vector should be built in the direct cross-product direction
        Triangle(Eigen::Vector3d position, Eigen::Vector3d point0, Eigen::Vector3d point1, Eigen::Vector3d point2, bool invert = false);

        /// @brief Build a triangle from three points in the global frame and infer the position of the triangle by baricentric coordinates
        /// @note The position of the triangle is set to the barycenter of the three points
        /// @param point0 The coordinates of the first point of the mesh in global frame (3-dim vector in meters)
        /// @param point1 The coordinates of the second point of the mesh in global frame (3-dim vector in meters)
        /// @param point2 The coordinates of the third point of the mesh in global frame (3-dim vector in meters)
        /// @param invert Wether the normal vector should be built in the direct cross-product direction
        Triangle(Eigen::Vector3d point0, Eigen::Vector3d point1, Eigen::Vector3d point2, bool invert = false)
            : Triangle((point0 + point1 + point2) / 3.0, point0, point1, point2, invert) {};


        /// @brief  A method to set the position of the camera in the global frame
        /// @param position The new position of the camera in the global frame
        /// @note The position is updated in the SceneObject class, and the pixel positions are updated accordingly
        void setPosition(const Eigen::Vector3d& position) {
            SceneObject::setPosition(position);
            updatePoints();
        };

        /// @brief A method to translate the camera along a displacement vector
        /// @param displacement The displacement vector
        void translate(const Eigen::Vector3d& displacement) {
            SceneObject::translate(displacement);
            updatePoints();
        };

        /// @brief A method to rotate the camera around a given axis
        /// @param axis The axis of rotation (must be a unit vector)
        void rotate(const Eigen::Vector3d& axis, const double angle) {
            SceneObject::rotate(axis, angle);
            updatePoints();
        };

        /// @brief A method to rotate the camera around a given rotation vector
        /// @param rotationVector The rotation vector (angle and axis of rotation)
        void rotate(const Eigen::Vector3d& rotationVector) {
            SceneObject::rotate(rotationVector);
            updatePoints();
        };



        /// @brief A method to get the vertex n° i of the triangle
        /// @param i The index of the vertex to get (must be within (0, 1, 2))
        /// @return The position vector of the vertex of the triangle in the global frame
        const Eigen::Vector3d& getPoint(int i) const;

        /// @brief A method to get the normal vector of the triangle
        /// @return The normal vector of the triangle in the global frame
        const Eigen::Vector3d& getNormal() const;

        /// @brief A method to get the bounding box of the triangle
        /// @return The bounding box of the triangle
        inline const Box& getBoundingBox() const { return m_bounding_box; }

        /// @brief Return true if the ray intersects the triangle
        /// @param R The Ray to test for intersection
        /// @param u The barycentric coordinate u of the intersection point (IP): IP = A + u * E1 + v * E2
        /// @param v The barycentric coordinate v of the intersection point (IP): IP = A + u * E1 + v * E2
        /// @param t The distance from the ray origin to the intersection point (IP): IP = R.Origin + t * R.Dir
        /// @return true if the Ray intersect the object, false otherwise
        bool intersectRay(const Ray& R, float& u, float& v, float& t) const;

    private:
        /// @brief The coordinates of the points of the triangle in the local frame
        Eigen::Vector3d m_point0;
        Eigen::Vector3d m_point1;
        Eigen::Vector3d m_point2;

        Eigen::Vector3d m_normal;


        /// @brief The global coordinates of the points of the triangle
        /// @note These points are the coordinates of the triangle in the global frame
        Eigen::Vector3d m_global_point0;
        Eigen::Vector3d m_global_point1;
        Eigen::Vector3d m_global_point2;

        Eigen::Vector3d m_global_normal;

        /// @brief The bounding box of the triangle in the global frame
        Box m_bounding_box;

        /// @brief A method to update the global points and normal vector based on the position and rotation of the triangle
        /// @note This method is called whenever the position or rotation of the triangle is changed
        void updatePoints();
};

