cmake_minimum_required(VERSION 3.24)

project(protobuf_)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PROTOBUF REQUIRED protobuf)
pkg_check_modules(ABSL REQUIRED absl_any)
pkg_check_modules(GTEST REQUIRED gtest gtest_main)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src src_list)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(protobuf_ ${src_list})
target_include_directories(protobuf_ PUBLIC ${PROTOBUF_INCLUDE_DIRS} ${ABSL_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
target_link_libraries(protobuf_ ${PROTOBUF_LIBRARIES} ${ABSL_LIBRARIES} ${GTEST_LIBRARIES})