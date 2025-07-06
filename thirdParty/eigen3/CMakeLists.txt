cmake_minimum_required(VERSION 3.10)
project(Eigen)

# Declare the Eigen library as an INTERFACE library
# This means it does not produce any binary output, but provides include directories
add_library(Eigen INTERFACE)

# Specify the include directories for Eigen
# This is where the Eigen headers are located
target_include_directories(Eigen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
