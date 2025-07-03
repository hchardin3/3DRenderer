#include <iostream>
#include <Eigen/Dense>
#include "src/camera.hpp"
#include "src/mesh.hpp"
#include "src/scene.hpp"
#include "src/utils.hpp"
#include "src/exporter.hpp"


int main() {

    Eigen::Vector3d A, B, C, D;
    A << -1., 1., 0.;
    B << 1., 1., 0.;
    C << 1., -1., 0.;
    D << -1., -1., 0.;

    Eigen::Vector3d position(0, 0, 3);

    // Create 2 triangles with the given points
    Triangle triangle(position, A, B, C);
    Triangle triangle2(position, A, C, D);

    Eigen::Vector3d cameraPosition(0, 0, 0);
    Camera myCam(cameraPosition, 1.7, 1.7, 800, 800, 1.0);
    myCam.rotate(Eigen::Vector3d::UnitY(), 0.5); // Rotate the camera 45 degrees around the Y-axis

    Scene myScene(&myCam);
    Render rdr = myScene.getRender();

    // std::cout << rdr.render << std::endl;

    myScene.addTriangle(&triangle);
    myScene.addTriangle(&triangle2);

    rdr = myScene.getRender();
    // std::cout << rdr.render << std::endl;

    auto exporter = MakeExporter(rdr.render);
    exporter.toBitmap("render.bmp");

    return 0;
}
 