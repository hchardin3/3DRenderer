#include "utils.hpp"

Render::Render(int verticalResolution, int horizontalResolution) :
    render(MatrixXchar::Zero(verticalResolution, horizontalResolution)) {
}

Render::Render(std::tuple<int, int> resolution) :
    render(MatrixXchar::Zero(std::get<0>(resolution), std::get<1>(resolution))) {
}

bool intersect_triangle(Ray R, Triangle triangle, float& t) {
    Eigen::Vector3d A = triangle.getPoint(0);
    Eigen::Vector3d B = triangle.getPoint(1);
    Eigen::Vector3d C = triangle.getPoint(2);

    Eigen::Vector3d E1 = B-A;
    Eigen::Vector3d E2 = C-A;
    Eigen::Vector3d N = E1.cross(E2);
    float det = -R.direction.dot(N);
    float invdet = 1.0/det;
    Eigen::Vector3d AO  = R.origin - A;
    Eigen::Vector3d DAO = AO.cross(R.direction);
    float u =  E2.dot(DAO) * invdet;
    float v = -E1.dot(DAO) * invdet;
    t =  AO.dot(N)  * invdet; 

    return (std::fabs(det) >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0);

}