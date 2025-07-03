cmake_minimum_required(VERSION 3.10)
project(3DRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find and include Eigen3 package
SET( EIGEN3_INCLUDE_DIR "Include\\eigen3" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

# Add the executables
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} main.cpp ${SRC_DIR}/sceneObject.cpp ${SRC_DIR}/mesh.cpp ${SRC_DIR}/utils.cpp ${SRC_DIR}/camera.cpp ${SRC_DIR}/light.cpp ${SRC_DIR}/scene.cpp)
# target_include_directories(MyExecutable PRIVATE ${SRC_DIR})

# Include Eigen3 directories
# target_include_directories(MyExecutable PRIVATE ${EIGEN3_INCLUDE_DIR})

# Optionally, link other libraries if needed
# target_link_libraries(MyExecutable SomeOtherLibrary)