#include "scene.hpp"

Scene::Scene(Camera* camera) :
    m_camera(camera) {}

Render Scene::getRender() {
    std::tuple<int, int> resolution = m_camera->getDimensions();
    Render my_render(resolution);

    for(int i(0); i<std::get<0>(resolution); ++i) {
        for(int j(0); j<std::get<1>(resolution); ++j) {
            Ray ray = m_camera->getRay(i, j);

            bool intersect(false);
            float t = 0.;
            for (const Triangle* triangle : m_listOfObjects) {
                if(intersect_triangle(ray, *triangle, t)) {
                    intersect = true;
                    break;
                }
            }      


            if(intersect) {my_render.render(i, j) = 255;}
        }
    }

    return my_render;
}

void Scene::addTriangle(Triangle* triangle) {
    m_listOfObjects.push_front(triangle);
}