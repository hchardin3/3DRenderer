cmake_minimum_required(VERSION 3.10)
project(3DRenderer)

# Chose the build type, which can be Debug or Release. This determines the optimization level and debug information included in the build.
# If not specified, it defaults to Release.
# This can be set in the command line with -DCMAKE_BUILD_TYPE=Debug or -DCMAKE_BUILD_TYPE=Release
# or in the CMake GUI.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Tell CMake where to find the source files for each third-party library (Need to contain a CMakeLists.txt)
add_subdirectory(thirdParty/eigen3)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Link the C++ standard library statically
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# MY_SOURCES is defined to be a list of all the source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Add the source files to the executable
add_executable(${PROJECT_NAME} ${MY_SOURCES})

# Add the include directory for headers
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include third-party libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen)

# Set the compiler options
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PUBLIC -g -O0)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PUBLIC -O3)
else()
    message(FATAL_ERROR "Unknown build type: '${CMAKE_BUILD_TYPE}'. Use 'Debug' or 'Release'.")
endif()