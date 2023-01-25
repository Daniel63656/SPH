cmake_minimum_required(VERSION 3.16)
project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        main.cpp
        datastructures/grid.cpp
        datastructures/particle.cpp
        kernelFunctions/cubicSpline.cpp
        kernelFunctions/gaussian.cpp
        settings/settings.cpp
        simulation/simulation.cpp)

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
