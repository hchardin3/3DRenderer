#pragma once

#include <list>
#include <tuple>
#include <Eigen/Dense>

#include "camera.hpp"
#include "light.hpp"
#include "triangle.hpp"
#include "Structures/ray.hpp"
#include "Structures/render.hpp"
#include "Structures/octree.hpp"

class Scene {
    private:
        Camera* m_camera;
        LightSource* m_lightSource;
        std::list<Triangle*> m_listOfObjects;
        Octree<Triangle> m_octree; // Octree to manage the scene objects efficiently
    
    public:
        /// @brief Create the whole scene that contains one camera and a few objects
        /// @param camera The camera to be used for rendering
        /// @param octree_max_depth The maximum depth of the octree
        /// @param octree_initial_size The initial size of the octree's root node (length of one side of the cube)
        /// @param octree_max_neighbors The maximum number of neighbors in each octree leaf
        Scene(Camera* camera, unsigned int octree_max_depth, double octree_initial_size, unsigned int octree_max_neighbors) : 
            m_camera(camera), 
            m_octree(octree_max_depth, octree_initial_size, octree_max_neighbors, camera->getPosition())
        {};

        /// @brief Set the light source of the scene
        /// @param lightSource The light source to be used for rendering
        void setLightSource(LightSource* lightSource) {
            m_lightSource = lightSource;
        }

        /// @brief This method analyses what the camera sees on each of its pixels
        /// @return The image frame of the scene through the camera's eye
        Render getRender() const;

        /// @brief A function to add an object to the scene
        /// @param triangle The object to be added (now only Triangle)
        void addTriangle(Triangle* triangle);
};