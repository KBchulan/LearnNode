cmake_minimum_required(VERSION 3.29)
project(async_api)

set(CMAKE_CXX_STANDARD 14)

add_executable(async_api
    src/main.cpp
        include/session.hpp
        src/session.cpp)
