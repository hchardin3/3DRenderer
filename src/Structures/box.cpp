#include "Structures/box.hpp"

// Taken from Tyron's anwer:
//      https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
// See for explanation:
//      https://tavianator.com/2011/ray_box.html
bool Box::intersect(const Ray& ray, double& t) const {
    const Eigen::Vector3d& origin = ray.getOrigin();
    const Eigen::Vector3d& inv_dir = ray.getInverseDirection();

    Eigen::Array3d min_diff = (min - origin.array()) * inv_dir.array();
    Eigen::Array3d max_diff = (max - origin.array()) * inv_dir.array();

    // Calculate the minimum and maximum t values for each axis
    double tmin = min_diff.min(max_diff).maxCoeff();
    double tmax = min_diff.max(max_diff).minCoeff();

    // Non branhing logic to determine the intersection distance `t`:
    //      If tmin < 0, that means the ray starts inside the box, so we want to return tmax
    //      If tmin > 0, that means the ray starts outside the box, so we want to return tmin
    //      t is only valid if tmax >= tmin && tmax >= 0, ie the ray intersects the box
    t = std::max(tmin, 0.0); // Ensure t is non-negative
    t = std::min(tmax, t); // Clamp t to the maximum intersection distance

    return tmax >= tmin && tmax >= 0;

    // Chip between the x-axis box and the ray
    // double t1 = (m_bounding_box.min.x() - origin.x())*inv_dir.x();
    // double t2 = (m_bounding_box.max.x() - origin.x())*inv_dir.x();

    // double tmin = std::min(t1, t2);
    // double tmax = std::max(t1, t2);

    // // Chip between the y-axis box and the ray
    // t1 = (m_bounding_box.min.y() - origin.y())*inv_dir.y();
    // t2 = (m_bounding_box.max.y() - origin.y())*inv_dir.y();

    // tmin = std::max(tmin, std::min(t1, t2));
    // tmax = std::min(tmax, std::max(t1, t2));

    // // Chip between the z-axis box and the ray
    // t1 = (m_bounding_box.min.z() - origin.z())*inv_dir.z();
    // t2 = (m_bounding_box.max.z() - origin.z())*inv_dir.z();

    // tmin = std::max(tmin, std::min(t1, t2));
    // tmax = std::min(tmax, std::max(t1, t2));
}