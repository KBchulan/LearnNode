cmake_minimum_required(VERSION 3.24)

project(coroutineServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

if(NOT JSONCPP_FOUND)
    message(FATAL_ERROR "jsoncpp library not found")
endif()

file(GLOB_RECURSE SRC_LIST 
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
        ${JSONCPP_LIBRARIES}
)