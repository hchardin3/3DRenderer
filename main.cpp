#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "src/camera.hpp"
#include "src/mesh.hpp"
#include "src/scene.hpp"
#include "src/utils.hpp"
#include "src/display.hpp"


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

    //Display myDisplay;
    //myDisplay.displayRenderAsImage(myScene);

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    printf("Rendering the scene...\n");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
 