cmake_minimum_required(VERSION 3.24)
project(Ch1-1)

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

find_package(glm REQUIRED)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FMT REQUIRED fmt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_FILES)

add_executable(${PROJECT_NAME} ${SRC_FILES}
        include/EasyVKstart.hpp
        include/VKBase.hpp
        include/GlfwGeneral.hpp)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 

    # 链接库
    glfw
    Vulkan::Vulkan
    ${FMT_LIBRARIES}
)