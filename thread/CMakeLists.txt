cmake_minimum_required(VERSION 3.25)

project(thread)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(thread src/main.cpp
        include/chrono.hpp
        include/thread.hpp)