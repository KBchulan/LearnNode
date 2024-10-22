cmake_minimum_required(VERSION 3.24)
project(c++17)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(c++17
    src/main.cpp
        include/c++17.hpp)
