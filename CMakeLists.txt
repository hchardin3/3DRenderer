cmake_minimum_required(VERSION 3.10)
project(3DRenderer)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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