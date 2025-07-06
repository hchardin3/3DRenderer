#include "scene.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;

Render Scene::getRender() const {
    auto t0 = high_resolution_clock::now();

    const std::tuple<const int, const int> dimensions = m_camera->getDimensions();
    const int verticalResolution = std::get<0>(dimensions);
    const int horizontalResolution = std::get<1>(dimensions);

    auto t1 = high_resolution_clock::now();

    Render my_render(verticalResolution, horizontalResolution);

    auto t2 = high_resolution_clock::now();

    const std::vector<Ray>& rays = m_camera->getRays(); // Generate rays from the camera

    auto t3 = high_resolution_clock::now();

    // Iterate through each pixel in the frame
    // Each pixel corresponds to a ray in the rays vector
    for (int linear_id(0); linear_id < verticalResolution * horizontalResolution; ++linear_id) 
    {
        const Ray& ray = rays[linear_id];

        bool intersect(false);
        float t, u, v;
        float t_opt; // Initialize t_opt to a small value
        const Triangle* hit_triangle = nullptr;
        for (const Triangle* triangle : m_listOfObjects) {
            if(intersect_triangle(ray, *triangle, u, v, t)) {
                if (!intersect || t < t_opt) {
                    t_opt = t; // Update the closest intersection distance
                    intersect = true;
                    hit_triangle = triangle;
                }
            }
        }


        if(intersect) {
            Eigen::Vector3d triangle_normal = hit_triangle->getNormal(); // Get the normal vector of the triangle
            Eigen::Vector3d hit_position = ray.getOrigin() + ray.getDirection() * t_opt; // Calculate the intersection point
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

                my_render.render(linear_id, 0) = intensity; // Set the pixel color in the render
                my_render.render(linear_id, 1) = intensity; // Set the pixel color in the render
                my_render.render(linear_id, 2) = intensity; // Set the pixel color in the render
            } else {
                my_render.render(linear_id, 0) = 10;
            }
        }
    }

    auto t4 = high_resolution_clock::now();

    std::cout << "Time taken for resolution: "
              << duration_cast<milliseconds>(t1 - t0).count() << " ms" << std::endl;
    std::cout << "Time taken for render: "
              << duration_cast<milliseconds>(t2 - t1).count() << " ms" << std::endl;
    std::cout << "Time taken for ray generation: "
                << duration_cast<milliseconds>(t3 - t2).count() << " ms" << std::endl;
    std::cout << "Time taken for intersection: "
                << duration_cast<milliseconds>(t4 - t3).count() << " ms" << std::endl;
    std::cout << "Total time taken: "
              << duration_cast<milliseconds>(t4 - t0).count() << " ms"  << std::endl;

    return my_render;
}

void Scene::addTriangle(Triangle* triangle) {
    m_listOfObjects.push_front(triangle);
}