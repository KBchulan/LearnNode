cmake_minimum_required(VERSION 3.24)

project(ATM)

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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/home/whx/qtware/5.14.2/gcc_64/lib/cmake")

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SPDLOG REQUIRED spdlog)
pkg_check_modules(MYSQLCLIENT REQUIRED mysqlclient)

find_package(Boost REQUIRED COMPONENTS filesystem system)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB HEADERS 
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
)

file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cc
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config.ini
    ${CMAKE_BINARY_DIR}/config/config.ini
    COPYONLY
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/style.qss
    ${CMAKE_BINARY_DIR}/resources/style.qss
    COPYONLY
)

add_executable(
    ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${SPDLOG_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    mysqlcppconn
    ${MYSQLCLIENT_LIBRARIES}
)