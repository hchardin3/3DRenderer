#include <iostream>
#include <Eigen/Dense>
#include "src/camera.hpp"
#include "src/mesh.hpp"
#include "src/scene.hpp"
#include "src/utils.hpp"
#include "src/exporter.hpp"


int main() {

    Eigen::Vector3d A, B, C, D;
    A << -1., 3., 1.;
    B << 1., 3., 1.;
    C << 1., 3., -1.;
    D << -1., 3., -1.;

    // Create 2 triangles with the given points
    Triangle triangle(A, B, C);
    Triangle triangle2(A, C, D);


    Camera myCam(1.7, 1.7, 800, 800, 1.0);
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
 