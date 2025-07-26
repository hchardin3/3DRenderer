#include <iostream>
#include <Eigen/Dense>
#include <math.h>

#include "camera.hpp"
#include "triangle.hpp"
#include "scene.hpp"
#include "exporter.hpp"
#include "light.hpp"

#include <chrono>
using namespace std::chrono;


int main() {
    constexpr unsigned int WIDTH = 800;
    constexpr unsigned int HEIGHT = 800;

    Eigen::Vector3d A, B, C, D;
    A << -1., 1., 0.;
    B << 1., 1., 0.;
    C << 1., -1., 0.;
    D << -1., -1., 0.;

    Eigen::Vector3d position(0, 0, 3);

    // Create 2 triangles with the given points
    Triangle triangle(position, A, B, C, true);
    // triangle.rotate(Eigen::Vector3d::UnitY(), M_PI); // Rotate the triangle around the Y-axis
    Triangle triangle2(position, A, C, D, true);

    // triangle2.rotate(Eigen::Vector3d::UnitY(), M_PI); // Rotate the second triangle around the Y-axis
    triangle2.rotate(Eigen::Vector3d::UnitX(), M_PI / 4); // Rotate the second triangle around the X-axis

    // Create a light source
    Eigen::Vector3d lightPosition(0, 5, 0);
    Eigen::Vector3d lightColor(1, 1, 1); // White light
    unsigned char lightIntensity(255); // Intensity of the light source
    LightSource light(lightPosition, lightColor, lightIntensity);

    // Create a camera
    // The camera is initialized at the origin, facing the Y-axis
    Eigen::Vector3d cameraPosition(0, 0, 0);
    const double horizontalFOV = 1.7; // Horizontal field of view in radians
    const double verticalFOV = 1.7; // Vertical field of view in radians
    const double projectionDistance = 1.0; // Distance from the camera to the projection plane
    Camera myCam(cameraPosition, horizontalFOV, verticalFOV, WIDTH, HEIGHT, projectionDistance);
    // myCam.rotate(Eigen::Vector3d::UnitY(), 0.5); // Rotate the camera around the Y-axis

    // Create a scene with the camera
    unsigned int octree_max_depth = 5;
    double octree_initial_size = 2.5; // Initial size of the octree's root node (length of one side of the cube)
    unsigned int octree_max_neighbors = 3; // Maximum number of neighbors in each octree leaf
    Scene myScene(&myCam, octree_max_depth, octree_initial_size, octree_max_neighbors);
    myScene.setLightSource(&light); // Set the light source in the scene

    // Add triangles to the scene
    myScene.addTriangle(&triangle);
    myScene.addTriangle(&triangle2);

    std::cout << "Scene created with triangles." << std::endl;

    auto start = high_resolution_clock::now();

    // Render the scene with the triangles
    Render rdr = myScene.getRender();
    // std::cout << rdr.render << std::endl;

    auto stop = high_resolution_clock::now();

    std::cout << "Render completed in "
              << duration_cast<milliseconds>(stop - start).count()
              << " milliseconds." << std::endl;

    // Export the render to a bitmap file
    auto exporter = MakeExporter(rdr.render, rdr.verticalResolution, rdr.horizontalResolution);

    std::cout << "Exporting render to bitmap..." << std::endl;
    
    exporter.toBitmap("render.bmp");

    return 0;
}
 