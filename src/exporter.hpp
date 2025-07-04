#pragma once

#include <iostream>
#include <fstream>  
#include <Eigen/Dense>

template<typename ArrayType>
class Exporter {
    private:
        const ArrayType& m_image;
        const int m_height, m_width;

    public:
        Exporter(const ArrayType& image, const int height, const int width) : 
                m_image(image), 
                m_height(height), m_width(width) {
            static_assert(ArrayType::ColsAtCompileTime == 3, "The image must have 3 channels (RGB)");

            assert(height > 0 && width > 0 && "The image dimensions must be positive");
            assert(m_image.rows() == height * width && "The given image dimensions do not match the image data");
            assert(m_image.cols() == 3 && "The image must have 3 channels (RGB)");
        }

        void toBitmap(const std::string& filename) const;
        void toCSV(const std::string& filename) const;
};

template <typename ArrayType>
Exporter<ArrayType> MakeExporter(const ArrayType& image, const int height, const int width) { return Exporter<ArrayType>(image, height, width); }

// Include the implementation file to make the template class implementation accessible to the compiler
#include "exporter.tpp"