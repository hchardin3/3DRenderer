#pragma once

#include <list>
#include <tuple>
#include <Eigen/Dense>

#include "camera.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "utils.hpp"
#include "Structures/render.hpp"

class Scene {
    private:
        Camera* m_camera;
        LightSource* m_lightSource;
        std::list<Triangle*> m_listOfObjects;
    
    public:
        /// @brief Create the whole scene that contains one camera and a few objects
        /// @param camera The camera to be used for rendering
        Scene(Camera* camera) : m_camera(camera) {};

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