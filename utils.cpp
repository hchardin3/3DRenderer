#include "utils.hpp"

Render::Render(int verticalResolution, int horizontalResolution) :
    render(MatrixXchar(verticalResolution, horizontalResolution)) {
}

Render::Render(std::tuple<int, int> resolution) :
    render(MatrixXchar(std::get<0>(resolution), std::get<1>(resolution))) {
}