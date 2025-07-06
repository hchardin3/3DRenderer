cmake_minimum_required(VERSION 3.10)
project(Eigen)

add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
