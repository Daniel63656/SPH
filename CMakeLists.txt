cmake_minimum_required(VERSION 3.16)
project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_executable(${PROJECT_NAME} ${source_files} ${header_files} src/utils/profiler.h)

# tell cmake to use the given precompiled header
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/pch.h)

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC pugixml)

find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)