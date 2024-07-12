#pragma once

#include <eigen3/Eigen/Dense>

class BasicMesh {
    private:
        Eigen::Vector3d m_point1;
        Eigen::Vector3d m_point2;
        Eigen::Vector3d m_point3;

        Eigen::Vector3d m_normal;
    
    public:
        /// @brief A very simple triangular mesh
        /// @param point1 The coordinates of the first point of the mesh (3-dim vector in meters)
        /// @param point2 The coordinates of the second point of the mesh (3-dim vector in meters)
        /// @param point3 The coordinates of the third point of the mesh (3-dim vector in meters)
        /// @param invert Wether the normal vector should be built in the direct cross-product direction
        BasicMesh(Eigen::Vector3d point1, Eigen::Vector3d point2, Eigen::Vector3d point3, bool invert = false);

};

