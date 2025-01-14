cmake_minimum_required(VERSION 3.24)

project(Window)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O3)
        add_compile_options(-march=native)
    endif()
endif()

find_package(glfw3 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FMT REQUIRED fmt)

add_executable(${PROJECT_NAME}
        src/main.cc
        src/glad/glad.h
        src/glad/glad.cc
        src/KHR/khrplatform.h
        src/config/config.h
        src/utils/utils.hpp)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        glfw
        ${FMT_LIBRARIES}
)
