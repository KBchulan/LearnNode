cmake_minimum_required(VERSION 3.24)
project(async_echo_cs_pro)

set(CMAKE_CXX_STANDARD 20)

add_executable(async_echo_cs_pro
        include/session.hpp
        src/session.cpp
        src/async_cs.cpp)
