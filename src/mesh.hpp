#pragma once

#include <Eigen/Dense>
#include <tuple>
#include <csignal>
#include "sceneObject.hpp"

class Triangle : public SceneObject {
    private:
        Eigen::Vector3d m_point0;
        Eigen::Vector3d m_point1;
        Eigen::Vector3d m_point2;

        Eigen::Vector3d m_normal;
    
    public:
        /// @brief A very simple triangular mesh
        /// @param point0 The coordinates of the first point of the mesh (3-dim vector in meters)
        /// @param point1 The coordinates of the second point of the mesh (3-dim vector in meters)
        /// @param point2 The coordinates of the third point of the mesh (3-dim vector in meters)
        /// @param invert Wether the normal vector should be built in the direct cross-product direction
        Triangle(Eigen::Vector3d position, Eigen::Vector3d point0, Eigen::Vector3d point1, Eigen::Vector3d point2, bool invert = false);

        /// @brief A method to get the vertex n° i of the triangle
        /// @param i The index of the vertex to get (must be within (0, 1, 2))
        /// @return The position vector of the vertex of the triangle in the global frame
        Eigen::Vector3d getPoint(int i) const;

};

