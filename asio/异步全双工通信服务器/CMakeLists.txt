cmake_minimum_required(VERSION 3.24)
project(async_batt_cs)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(async_batt_cs
    src/async_cs.cpp
        include/CServer.hpp
        include/CSession.hpp
        src/CServer.cpp
        src/CSession.cpp)
