#include "scene.hpp"

Scene::Scene(Camera* camera) :
    m_camera(camera) {}

Render Scene::getRender() {
    std::tuple<int, int> resolution = m_camera->getDimensions();
    Render my_render(resolution);

    return my_render;
}