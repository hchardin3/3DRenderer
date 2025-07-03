#include "scene.hpp"

Scene::Scene(Camera* camera) :
    m_camera(camera) {}

Render Scene::getRender() const {
    std::tuple<int, int> resolution = m_camera->getDimensions();
    Render my_render(resolution);

    for(int i(0); i<std::get<0>(resolution); ++i) {
        for(int j(0); j<std::get<1>(resolution); ++j) {
            Ray ray = m_camera->getRay(i, j);

            bool intersect(false);
            float t, u, v;
            Triangle* hit_triangle = nullptr;
            for (const Triangle* triangle : m_listOfObjects) {
                if(intersect_triangle(ray, *triangle, u, v, t)) {
                    intersect = true;
                    hit_triangle = const_cast<Triangle*>(triangle); // Cast away constness to modify the triangle
                    break;
                }
            }


            if(intersect) {
                Eigen::Vector3d triangle_normal = hit_triangle->getNormal(); // Get the normal vector of the triangle
                Eigen::Vector3d hit_position = ray.origin + ray.direction * t; // Calculate the intersection point
                Eigen::Vector3d lightDirection = m_lightSource->getPosition() - hit_position;
                lightDirection.normalize(); // Normalize the light direction vector

                // Calculate the dot product between the triangle normal and the light direction
                float dotProduct = triangle_normal.dot(lightDirection);
                // If the dot product is positive, the triangle is lit by the light source
                if (dotProduct > 0) {
                    // Calculate the color intensity based on the dot product
                    unsigned char intensity = dotProduct * m_lightSource->getIntensity();
                    // std::cout << "Pixel (" << i << ", " << j << ") is lit with intensity: " << static_cast<int>(intensity) << std::endl;
                    // std::cout << m_lightSource->getIntensity() << std::endl;
                    my_render.render(i, j) = intensity; // Set the pixel color in the render
                } else {
                    my_render.render(i, j) = 0; // If not lit, set pixel to black
                }
            }
        }
    }

    return my_render;
}

void Scene::addTriangle(Triangle* triangle) {
    m_listOfObjects.push_front(triangle);
}