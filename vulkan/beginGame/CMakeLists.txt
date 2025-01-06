cmake_minimum_required(VERSION 3.20)
project(vulkan_cuda_game CUDA CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 17)

# 查找必要的包
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(CUDA REQUIRED)

set(REQUIRED_VULKAN_EXTENSIONS
    VK_KHR_swapchain
)

# 修改编译 SPIR-V 着色器部分
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)

find_program(GLSL_VALIDATOR glslangValidator REQUIRED)

set(SHADER_SOURCES
    ${CMAKE_SOURCE_DIR}/shaders/shader.vert
    ${CMAKE_SOURCE_DIR}/shaders/shader.frag
)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv
        COMMAND ${GLSL_VALIDATOR} -V ${SHADER} -o ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${SHADER_NAME}"
    )
    list(APPEND SPV_SHADERS ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv)
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

# 添加源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.cu"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} shaders)

# 设置包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
    ${Vulkan_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    cuda
    cudart
)

# 在项目设置后添加
if(CMAKE_CUDA_COMPILER)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler -Wno-attributes")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -diag-suppress 20012")
    
    # 如果使用的是 NVIDIA HPC SDK (nvc++)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "NVIDIA")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Minform=inform")
    endif()
endif()

# 为 C++ 编译器添加警告抑制
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

# 在 target_link_libraries 之后添加
if(UNIX)
    # 使用 CUDA 的实际路径
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES}"
    )
endif() 