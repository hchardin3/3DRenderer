#include <iostream>
#include <Eigen/Dense>
#include "src/camera.hpp"
#include "src/mesh.hpp"
#include "src/scene.hpp"
#include "src/utils.hpp"


int main() {

    Eigen::Vector3d A, B, C;
    A << -1., 3., 1.;
    B << 1., 3., 1.;
    C << 0.5, 3., -1.;

    Triangle triangle(A, B, C);


    Camera myCam(1.5, 1.5, 25, 25, 1.0);
    Scene myScene(&myCam);
    Render rdr = myScene.getRender();

    std::cout << rdr.render << std::endl;

    std::cout << "\n \nNow the testing" << std::endl;

    

    myScene.addTriangle(&triangle);

    rdr = myScene.getRender();
    std::cout << rdr.render << std::endl;

    return 0;
}
 