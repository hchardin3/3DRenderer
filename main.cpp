#include <iostream>
#include <eigen3/Eigen/Dense>
#include "camera.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "utils.hpp"




int main() {
    Camera myCam(90.0, 90.0, 25, 10, 1.0);
    Scene myScene(&myCam);
    Render rdr = myScene.getRender();

    std::cout << rdr.render << std::endl;

    return 0;
}
 