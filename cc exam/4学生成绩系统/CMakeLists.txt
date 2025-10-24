cmake_minimum_required(VERSION 3.24)

project(StudentGradeManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB HEADERS 
    ${PROJECT_SOURCE_DIR}/include/*.h
)

file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cc
)

add_executable(${PROJECT_NAME} 
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets  
)