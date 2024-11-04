cmake_minimum_required(VERSION 3.24)

project(sync_cs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

add_executable(client src/client.cpp)
add_executable(server src/server.cpp)
