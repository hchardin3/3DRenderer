#include "utils.hpp"

//
// The intersect_triangle method is based on the algorithm described by BrunoLevy at:
// https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d
//
// Explanation of the algorithm:
//      https://tavianator.com/2014/ray_triangle.html
bool intersect_triangle(const Ray& R, const Triangle* triangle, float& u, float& v, float& t) {
    // AABB (Axis-Aligned Bounding Box) check
    // If the ray does not intersect the bounding box of the triangle, return false
    if (!triangle->AABB_intersect(R)) {
        return false;
    }
    
    const Eigen::Vector3d& A = triangle->getPoint(0);
    const Eigen::Vector3d& B = triangle->getPoint(1);
    const Eigen::Vector3d& C = triangle->getPoint(2);

    Eigen::Vector3d E1 = B-A;
    Eigen::Vector3d E2 = C-A;
    Eigen::Vector3d N = E1.cross(E2);
    float det = -R.getDirection().dot(N);
    float invdet = 1.0/det;
    Eigen::Vector3d AO  = R.getOrigin() - A;
    Eigen::Vector3d DAO = AO.cross(R.getDirection());

    // Calculate the barycentric coordinates of the intersection (u, v, 1-u-v) 
    u =  E2.dot(DAO) * invdet;
    v = -E1.dot(DAO) * invdet;

    // Calculate the distance from the ray origin to the intersection point
    t = AO.dot(N) * invdet;

    return (std::fabs(det) >= 1e-6 && t >= 0 && u >= 0 && v >= 0 && (u+v) <= 1);
}
