cmake_minimum_required(VERSION 3.29)
project(c++20)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(c++20
    src/main.cpp
        include/c++20.hpp)
