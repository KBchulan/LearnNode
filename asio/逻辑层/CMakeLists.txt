cmake_minimum_required(VERSION 3.24)

project(LogicServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(LogicServer ${SRC_LIST}
        src/async_server.cc
        include/MsgNode.hpp
        src/MsgNode.cc
        include/config.hpp
        include/CSession.hpp
        include/CServer.hpp
        src/CServer.cc
)
target_link_libraries(LogicServer ${JSONCPP_LIBRARIES})