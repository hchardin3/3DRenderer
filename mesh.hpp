#include <eigen3/Eigen/Dense>

class BasicMesh {
    private:
        Eigen::Vector3d m_point1;
        Eigen::Vector3d m_point2;
        Eigen::Vector3d m_point3;

        Eigen::Vector3d m_normal;
    
    public:
        BasicMesh(Eigen::Vector3d point1, Eigen::Vector3d point2, Eigen::Vector3d point3, bool invert = false);

};

