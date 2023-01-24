cmake_minimum_required(VERSION 3.16)
project("SPH" VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_executable(${PROJECT_NAME}
        ${source_files}
        ${header_files})

# Add the project directory to include directories, to be able to include all project header files from anywhere
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
