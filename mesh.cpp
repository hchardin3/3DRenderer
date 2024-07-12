#include "mesh.hpp"

BasicMesh::BasicMesh(Eigen::Vector3d point1, Eigen::Vector3d point2, Eigen::Vector3d point3, bool invert) :
    m_point1(point1),
    m_point2(point2),
    m_point3(point3) {
        Eigen::Vector3d a{point1 - point2};
        Eigen::Vector3d b{point3 - point2};

        m_normal = invert ? -a.cross(b) : a.cross(b);
    }