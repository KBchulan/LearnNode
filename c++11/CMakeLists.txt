cmake_minimum_required(VERSION 3.25)

project(c++11)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(c++11 ${SRC_LIST}
        include/smart_ptr.hpp
        include/type.hpp)