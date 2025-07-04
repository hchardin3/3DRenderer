#include <iostream>
#include <Eigen/Dense>
#include <math.h>
#include "src/camera.hpp"
#include "src/mesh.hpp"
#include "src/scene.hpp"
#include "src/utils.hpp"
#include "src/exporter.hpp"
#include "src/light.hpp"

#include <chrono>
using namespace std::chrono;


int main() {

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
    Eigen::Vector3d lightPosition(0, 0, 0);
    Eigen::Vector3d lightColor(1, 1, 1); // White light
    unsigned char lightIntensity(255); // Intensity of the light source
    LightSource light(lightPosition, lightColor, lightIntensity);

    // Create a camera
    // The camera is initialized at the origin, facing the Y-axis
    Eigen::Vector3d cameraPosition(0, 0, 0);
    Camera myCam(cameraPosition, 1.7, 1.7, 800, 800, 1.0);
    // myCam.rotate(Eigen::Vector3d::UnitY(), 0.5); // Rotate the camera around the Y-axis

    // Create a scene with the camera
    Scene myScene(&myCam);
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
    auto exporter = MakeExporter(rdr.render);

    std::cout << "Exporting render to bitmap..." << std::endl;
    
    exporter.toBitmap("render.bmp");

    return 0;
}
 