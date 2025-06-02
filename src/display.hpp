#pragma once

#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "scene.hpp"

class Display {
    public:
        void displayRenderAsImage(const Scene& scene);
};