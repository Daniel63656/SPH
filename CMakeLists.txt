cmake_minimum_required(VERSION 3.16)

# Find OpenMP
if(APPLE)
    
    OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)
    set(CMAKE_CXX_COMPILER "/usr/local/Cellar/llvm/15.0.6/bin/clang++")
    set(OPENMP_LIBRARIES "/usr/local/Cellar/llvm/15.0.6/lib")
    set(OPENMP_INCLUDES "/usr/local/Cellar/llvm/15.0.6/include")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
      set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
      set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
      set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_executable(${PROJECT_NAME} ${source_files} ${header_files})

# tell cmake to use the given precompiled header
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/pch.h)

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC pugixml::pugixml)

find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
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
