cmake_minimum_required(VERSION 3.16)
project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        main.cpp
        simulation/simulation.cpp
        datastructures/grid.cpp
        kernelFunctions/cubicSpline.cpp
        kernelFunctions/gaussian.cpp
        settings/settings.cpp
        output_writer/vtk_writer.cpp)

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Search for the external package "VTK"
find_package(VTK)

# Output various cmake variables for demonstration purpose
message("If VTK was found on the system: VTK_FOUND: ${VTK_FOUND}")
message("The directory of VTK: VTK_DIR: ${VTK_DIR}")
message("The include directory of VTK: VTK_INCLUDE_DIRS: ${VTK_INCLUDE_DIRS}")

# If an installation of vtk was found on the system
if (VTK_FOUND)
    include_directories(${VTK_INCLUDE_DIRS})               # add the include directory where the header files are for the compiler
    target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES}) # add the libraries for the linker
endif(VTK_FOUND)