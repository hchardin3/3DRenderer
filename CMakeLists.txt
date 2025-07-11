cmake_minimum_required(VERSION 3.10)
project(3DRenderer)

# Chose the build type, which can be Debug or Release. This determines the optimization level and debug information included in the build.
# If not specified, it defaults to Release.
# This can be set in the command line with -DCMAKE_BUILD_TYPE=Debug or -DCMAKE_BUILD_TYPE=Release
# or in the CMake GUI.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


### PROJECT CONFIGURATION ###

# Tell CMake where to find the source files for each third-party library (Need to contain a CMakeLists.txt)
add_subdirectory(thirdParty/eigen3)
add_subdirectory(thirdParty/doctest)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Link the C++ standard library statically
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


### CUSTOM LIBRARY DEFINITION ###

# Macro from user20039963's answer on StackOverflow:
#       https://stackoverflow.com/questions/32016750/cmake-exclude-files-from-a-given-pattern-after-fileglob-recurse
#######
# Define a macro to recursively add source files from a directory and exclude certain files
# This macro takes four arguments:
# 1. retVal: The name of the variable to store the resulting list of files
# 2. dir: The directory to search for source files
# 3. ext: The file extension to look for (e.g., "*.cpp")
# 4. excl: A regular expression to exclude certain files from the list
# Usage: add_recursive(MY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "main.cpp")
# This will populate MY_SOURCES with all .cpp files in the src directory, excluding main.cpp.
macro( add_recursive retVal dir ext excl)
    # Store unfiltered list of files into temporary list
    file( GLOB_RECURSE _tmp_list CONFIGURE_DEPENDS ${dir}/${ext})
    # Resulted list is initially empty. Copy all needed elements into it.
    set(${retval} "")
    foreach(f ${_tmp_list})
        if(NOT f MATCHES ${excl})
            list(APPEND ${retVal} ${f})
        endif()
    endforeach()
endmacro( add_recursive )

# MY_SOURCES is defined to be a list of all the source files besides main.cpp
add_recursive(MY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "main.cpp")

# Create a library from the source files
# This library will be used by the executable and can be linked to other projects, including tests.
# The library is static, meaning it will be linked into the executable at compile time.
add_library(MY_LIB STATIC ${MY_SOURCES})

# Add the include directories for the library
# This allows the library to find its header files when included in other source files.
target_include_directories(MY_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link the third-party libraries to the MY_LIB library
target_link_libraries(MY_LIB PUBLIC Eigen)



### PROJECT EXECUTABLE ###

# Add the source files to the executable
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Add the custom library to the executable
# This links the MY_LIB library to the executable, allowing it to use the functions and classes defined in MY_LIB.
target_link_libraries(${PROJECT_NAME} PRIVATE MY_LIB)

# Include third-party libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen)



### COMPILER OPTIONS ###

# Set the compiler options
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PUBLIC -g -O0)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PUBLIC -O3)
else()
    message(FATAL_ERROR "Unknown build type: '${CMAKE_BUILD_TYPE}'. Use 'Debug' or 'Release'.")
endif()



### TESTS CONFIGURATION ###

# Find all the test source files
add_recursive(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/src "*.cpp" "main.cpp")

# Add the test executable
add_executable(${PROJECT_NAME}_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/main.cpp ${TEST_SOURCES})

# Add the test source files headers for the tests
target_include_directories(${PROJECT_NAME}_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/include)

# Add the custom library to the test executable
# This links the MY_LIB library to the test executable, allowing it to use the functions and classes defined in MY_LIB.
# The test executable will use the same library as the main project.
target_link_libraries(${PROJECT_NAME}_tests PRIVATE MY_LIB)

# Add third-party libraries to the test executable
target_link_libraries(${PROJECT_NAME}_tests PRIVATE doctest::doctest Eigen)

# Add compiler options for the test executable
target_compile_options(${PROJECT_NAME}_tests PUBLIC -Wall -Wextra -g -O0)
