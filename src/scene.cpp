#include "scene.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;

Render Scene::getRender() const {
    // auto t0 = high_resolution_clock::now();

    const std::tuple<const unsigned int, const unsigned int> dimensions = m_camera->getDimensions();
    const unsigned int verticalResolution = std::get<0>(dimensions);
    const unsigned int horizontalResolution = std::get<1>(dimensions);

    // auto t1 = high_resolution_clock::now();

    Render my_render(verticalResolution, horizontalResolution);

    // auto t2 = high_resolution_clock::now();

    const std::vector<Ray>& rays = m_camera->getRays(); // Generate rays from the camera

    // auto t3 = high_resolution_clock::now();

    // auto tracing_time = 0.0;

    // Iterate through each pixel in the frame
    // Each pixel corresponds to a ray in the rays vector
    for (int linear_id(0); linear_id < verticalResolution * horizontalResolution; ++linear_id) 
    {
        const Ray& ray = rays[linear_id];

        double hit_distance;
        // auto t_start = high_resolution_clock::now();
        const Triangle* hit_triangle = m_octree.traceRay(ray, hit_distance); // Check for intersection with the octree
        // auto t_end = high_resolution_clock::now();
        // tracing_time += duration_cast<nanoseconds>(t_end - t_start).count(); // Measure the time taken for ray tracing

        // If a triangle was hit, calculate the color intensity based on the light source
        if(hit_triangle) {
            Eigen::Vector3d triangle_normal = hit_triangle->getNormal(); // Get the normal vector of the triangle
            Eigen::Vector3d hit_position = ray.getOrigin() + ray.getDirection() * hit_distance; // Calculate the intersection point
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
                my_render.render(linear_id, 0) = 50;
            }
        }
    }

    // auto t4 = high_resolution_clock::now();

    // std::cout << "Time taken for resolution: "
    //           << duration_cast<milliseconds>(t1 - t0).count() << " ms" << std::endl;
    // std::cout << "Time taken for render: "
    //           << duration_cast<milliseconds>(t2 - t1).count() << " ms" << std::endl;
    // std::cout << "Time taken for ray generation: "
    //             << duration_cast<milliseconds>(t3 - t2).count() << " ms" << std::endl;
    // std::cout << "Time taken for intersection: "
    //             << duration_cast<milliseconds>(t4 - t3).count() << " ms" << std::endl;
    // std::cout << "Total time taken for ray tracing: "
    //           << tracing_time * 1e-6 << " ms"
    //           << " (" << tracing_time / duration_cast<nanoseconds>(t4 - t0).count() * 100 << " % of total time)" << std::endl;
    // std::cout << "Total time taken: "
    //           << duration_cast<milliseconds>(t4 - t0).count() << " ms"  << std::endl;

    return my_render;
}

void Scene::addTriangle(Triangle* triangle) {
    m_octree.insert(triangle); // Insert the triangle into the octree
}