cmake_minimum_required(VERSION 3.29)
project(c++14)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(c++14
    src/main.cpp
        include/c++14.hpp)
