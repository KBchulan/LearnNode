cmake_minimum_required(VERSION 3.24)

project(client)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)

add_executable(client ${SRC_LIST})
target_link_libraries(client ${JSONCPP_LIBRARIES})