#pragma once

#include <iostream>
#include <fstream>  
#include <Eigen/Dense>

template<typename ArrayType>
class Exporter {
    private:
        const ArrayType& m_image;

    public:
        Exporter(const ArrayType& image) : m_image(image) {};

        void toBitmap(const std::string& filename) const;
        void toCSV(const std::string& filename) const;
};

template <typename ArrayType>
Exporter<ArrayType> MakeExporter(const ArrayType& image) { return Exporter<ArrayType>(image); }

// Include the implementation file to make the template class implementation accessible to the compiler
#include "exporter.tpp"