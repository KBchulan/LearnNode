cmake_minimum_required(VERSION 3.24)
project(async_cs)

set(CMAKE_CXX_STANDARD 20)

add_executable(async_cs
        include/session.hpp
        src/session.cpp
        src/async_cs.cpp)
