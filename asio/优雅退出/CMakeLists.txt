cmake_minimum_required(VERSION 3.24)

project(exit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(exit ${SRC_LIST}
        include/Singleton.hpp
        include/LogicSystem.hpp
        include/LogicNode.hpp
        src/LogicNode.cc
        src/LogicSystem.cc)

target_link_libraries(exit ${JSONCPP_LIBRARIES})
