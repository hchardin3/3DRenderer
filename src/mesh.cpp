#include "mesh.hpp"

Triangle::Triangle(Eigen::Vector3d position, Eigen::Vector3d point0, Eigen::Vector3d point1, Eigen::Vector3d point2, bool invert) :
        SceneObject(position, Eigen::Vector3d::UnitY(), Eigen::Vector3d::UnitZ()),
        m_point0(point0),
        m_point1(point1),
        m_point2(point2) 
{
    Eigen::Vector3d a{point0 - point1};
    Eigen::Vector3d b{point2 - point1};

    m_normal = invert ? -a.cross(b) : a.cross(b);
    if (m_normal.norm() == 0) {
        throw std::invalid_argument("The points of the triangle must not be collinear.");
    }
    m_normal.normalize(); // Ensure the normal vector is normalized
}

Eigen::Vector3d Triangle::getPoint(int i) const {
    Eigen::Vector3d point;
    switch (i)
    {
    case 0: point = m_point0; break;
    case 1: point = m_point1; break;
    case 2: point = m_point2; break;
    
    default:
        std::raise(SIGSEGV);
        return Eigen::Vector3d::Zero();
    }

    return m_position + getRotationMatrix().inverse() * point;
}

const Eigen::Vector3d Triangle::getNormal() const { 
    // Rotate the normal vector by the object's rotation
    return getRotationMatrix() * m_normal;
}