cmake_minimum_required(VERSION 3.16)
project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_LIST_DIR}/*.h")

add_executable(${PROJECT_NAME} ${source_files} ${header_files})

# tell cmake to use the given precompiled header
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/pch.h)

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC pugixml::pugixml)

# Search for the external package "VTK"
#find_package(VTK)

# Output various cmake variables for demonstration purpose
#message("If VTK was found on the system: VTK_FOUND: ${VTK_FOUND}")
#message("The directory of VTK: VTK_DIR: ${VTK_DIR}")
#message("The include directory of VTK: VTK_INCLUDE_DIRS: ${VTK_INCLUDE_DIRS}")

# If an installation of vtk was found on the system
#if (VTK_FOUND)
#    include_directories(${VTK_INCLUDE_DIRS})               # add the include directory where the header files are for the compiler
#    target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES}) # add the libraries for the linker
#endif(VTK_FOUND)
