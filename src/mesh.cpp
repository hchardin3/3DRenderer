#include "mesh.hpp"

Triangle::Triangle(Eigen::Vector3d point0, Eigen::Vector3d point1, Eigen::Vector3d point2, bool invert) :
        m_point0(point0),
        m_point1(point1),
        m_point2(point2) 
{
    Eigen::Vector3d a{point0 - point1};
    Eigen::Vector3d b{point2 - point1};

    m_normal = invert ? -a.cross(b) : a.cross(b);
}

Eigen::Vector3d Triangle::getPoint(int i) const {
    switch (i)
    {
    case 0: return m_point0;
    case 1: return m_point1;
    case 2: return m_point2;
    
    default:
        std::raise(SIGSEGV);
        return Eigen::Vector3d::Zero();
    }
}