cmake_minimum_required(VERSION 3.15)
project(OpenCVProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(opencv REQUIRED)

add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE opencv::opencv)