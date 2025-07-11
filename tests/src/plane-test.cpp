#include "plane-test.hpp"

bool approximatelyEqual(double a, double b, double epsilon = std::numeric_limits<double>::epsilon()) {
    return std::fabs(a - b) < epsilon;
}

TEST_CASE("[Plane] testing plane construction") {
    Eigen::Vector3d normal(0, 1, 1); // Normal vector of the plane
    Eigen::Vector3d point(0, 0, 0); // A point on the plane
    Plane plane(normal, point);

    SUBCASE("Plane normal is normalized") {
        CHECK(approximatelyEqual(plane.normal.norm(), 1.0));
    }

    SUBCASE("Plane point is correctly set") {
        CHECK(plane.point == point);
    }

    SUBCASE("Plane point dot normal is correctly calculated") {
        double expected_dot = point.dot(normal);
        CHECK(approximatelyEqual(plane.getPointDotNormal(), expected_dot));
    }

    SUBCASE("Plane construction with zero normal vector") {
        Eigen::Vector3d zero_normal(0, 0, 0);
        CHECK_THROWS_AS(Plane zero_plane(zero_normal, point), std::invalid_argument);
    }
}

TEST_CASE("[Plane] testing plane intersection with ray") {
    Eigen::Vector3d normal(0, 1, 0); // Normal vector of the plane
    Eigen::Vector3d point(0, 0, 0); // A point on the plane
    Plane plane(normal, point);

    SUBCASE("Ray intersects the plane from below") {
        Ray ray(Eigen::Vector3d(0, -1, 0), Eigen::Vector3d(0, 1, 0)); // Ray pointing towards the plane from below
        double t;
        CHECK(plane.intersect(ray, t));
        CHECK(approximatelyEqual(t, 1.0)); // The intersection point should be at t = 1.0
    }

    SUBCASE("Ray intersects the plane from above") {
        Ray ray(Eigen::Vector3d(0, 2.4, 0), Eigen::Vector3d(0, -1, 0)); // Ray pointing towards the plane from above
        double t;
        CHECK(plane.intersect(ray, t));
        CHECK(approximatelyEqual(t, 2.4)); // The intersection point should be at t = 1.0
    }

    SUBCASE("Ray does not intersect the plane") {

        SUBCASE("Ray parallel to the plane") {
            // Ray parallel to the plane and above it
            Ray ray(Eigen::Vector3d(0, 1, 0), Eigen::Vector3d(1, 0, 0)); // Ray parallel to the plane
            double t;
            CHECK_FALSE(plane.intersect(ray, t));
        }

        SUBCASE("Ray pointing away from the plane") {
            // Ray pointing away from the plane
            Ray ray(Eigen::Vector3d(0, 1, 0), Eigen::Vector3d(0, 1, 1)); // Ray pointing away from the plane
            double t;
            CHECK_FALSE(plane.intersect(ray, t));
        }
    }

    SUBCASE("Ray origin on the plane") {
        SUBCASE("Ray point upwards from the plane") {
            // Ray origin is on the plane
            Ray ray(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 1, 0)); // Ray starting on the plane
            double t;
            CHECK(plane.intersect(ray, t));
            CHECK(approximatelyEqual(t, 0.0)); // The intersection point should be at t = 0.0
        }

        SUBCASE("Ray point downwards from the plane") {
            // Ray origin is on the plane
            Ray ray(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, -1, 0)); // Ray starting on the plane
            double t;
            CHECK(plane.intersect(ray, t));
            CHECK(approximatelyEqual(t, 0.0)); // The intersection point should be at t = 0.0
        }

        SUBCASE("Ray point parallel to the plane") {
            // Ray origin is on the plane and parallel to the plane
            Ray ray(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 0, 0)); // Ray starting on the plane
            double t;
            CHECK_FALSE(plane.intersect(ray, t)); // The ray is parallel to the plane, so no intersection
        }
    }
}