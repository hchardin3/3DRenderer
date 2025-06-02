cmake_minimum_required(VERSION 3.10)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(3DRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/eigen3)
set(SFML_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Include/SFML261)

# Find and include Eigen3 package
# IF( NOT EIGEN3_INCLUDE_DIR )
#     MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
# ENDIF()

set(SFML_DIR "${SFML_DIRECTORY}/lib/cmake/SFML")
set(SFML_LIBRARY_DIR ${SFML_DIR}/lib)
set(SFML_INCLUDE_DIR ${SFML_DIR}/include)

# Add the executables
add_executable(${PROJECT_NAME} main.cpp ${SRC_DIR}/mesh.cpp ${SRC_DIR}/utils.cpp ${SRC_DIR}/camera.cpp ${SRC_DIR}/scene.cpp ${SRC_DIR}/display.cpp)

list(APPEND CMAKE_MODULE_PATH ${SFML_DIR})
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  # SFML version >= 2.5
  #In SFML Version 2.5 or later, ${SFML_LIBRARIES}, ${SFML_DEPENDENCIES}, ${SFML_INCLUDE_DIR} no longer exist. Specify libraries individually.
  target_link_libraries(${PROJECT_NAME} sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
  message(STATUS "SFML found: ${SFML_VERSION}")
  # target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARY_DIR}/libsfml-network-s.a ${SFML_DIRECTORY}/lib/libsfml-audio-s.a ${SFML_DIRECTORY}/lib/libsfml-graphics-s.a ${SFML_DIRECTORY}/lib/libsfml-window-s.a ${SFML_DIRECTORY}/lib/libsfml-system-s.a)
  # find_package(${PROJECT_NAME} ${SFML_DIR}/libsfml-network.a)
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
# set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)



# find_library(SFML_LIBRARY sfml-main PATHS ${SFML_LIBRARY_DIR})
# target_link_libraries(MyExecutable PUBLIC ${SFML_LIBRARY})

# add_library(SFML STATIC IMPORTED) # or STATIC instead of SHARED
# set_target_properties(SFML PROPERTIES
#   IMPORTED_LOCATION ${SFML_LIBRARY_DIR}
#   INTERFACE_INCLUDE_DIRECTORIES ${SFML_INCLUDE_DIR}
# )
# target_link_libraries(MyExecutable PUBLIC SFML)

# find_library(SFML_GRAPHICS_LIBRARY sfml-graphics sfml-main sfml-window sfml-system PATHS ${SFML_LIBRARY_DIR})

# find_library(SFML_GRAPHICS_LIBRARY sfml-main-d sfml-graphics-d sfml-window-d sfml-system-d vorbis openal32 ogg freetype FLAC PATHS ${SFML_LIBRARY_DIR})

# Link SFML libraries (for linker)
#target_link_libraries(${PROJECT_NAME} PUBLIC ${SFML_GRAPHICS_LIBRARY})

# Include SFML directories (header files)
#target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})

# Include Eigen3 directories (no need to link Eigen3 libraries since it is a header-only library)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})

message(${SFML_DIR})
message(${SFML_LIBRARY_DIR})
message(${SFML_INCLUDE_DIR})
message(${SFML_GRAPHICS_LIBRARY})