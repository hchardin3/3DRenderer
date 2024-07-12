#pragma once

#include "camera.hpp"
#include "mesh.hpp"
#include <list>
#include <tuple>
#include <eigen3/Eigen/Dense>
#include "utils.hpp"

class Scene {
    private:
        Camera* m_camera;
        std::list<Triangle*> m_listOfObjects;
    
    public:
        /// @brief Create the whole scene that contains one camera and a few objects
        /// @param camera The camera to be used for rendering
        Scene(Camera* camera);

        Render getRender();

        void addTriangle(Triangle* triangle);


};