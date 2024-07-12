#include <eigen3/Eigen/Dense>

class Camera {
    private:
        Eigen::Vector3d m_position;
        Eigen::Vector3d m_up;
        Eigen::Vector3d m_forward;

        double m_horizontalFOV;
        double m_verticalFOV;
        double m_horizontalResolution;
        double m_verticalResolution;
        double m_distance;

        Eigen::MatrixXd m_renderer;


    public:
        Camera(double horizontalFOV, double verticalFOV,  double horizontalResolution,  double verticalResolution,  double distance);

};
