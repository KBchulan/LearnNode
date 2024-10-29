cmake_minimum_required(VERSION 3.24)

project(mutab_pkg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(mutab_pkg ${SRC_LIST}
        src/CSession.cpp)