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

    updatePoints(); // Initialize global points and normal
}

void Triangle::updatePoints() {
    // The inverse rotation matrix is the transpose of the rotation matrix
    Eigen::Matrix3d inv_rot = getRotationMatrix().transpose();

    // Update the global points based on the position and rotation of the triangle
    m_global_point0 = m_position + inv_rot * m_point0;
    m_global_point1 = m_position + inv_rot * m_point1;
    m_global_point2 = m_position + inv_rot * m_point2;

    // Update the global normal vector
    m_global_normal = inv_rot * m_normal;
}

const Eigen::Vector3d& Triangle::getPoint(int i) const {
    switch (i)
    {
    case 0: return m_global_point0;
    case 1: return m_global_point1;
    case 2: return m_global_point2;
    
    default:
        std::raise(SIGSEGV);
        return m_global_point0; // This line will never be reached, but it avoids a warning
    }
}

const Eigen::Vector3d& Triangle::getNormal() const { 
    // Rotate the normal vector by the object's rotation
    return m_global_normal;
}