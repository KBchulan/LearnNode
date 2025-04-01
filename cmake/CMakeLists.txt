# https://cmake.org/cmake/help/latest/guide/tutorial/index.html

# 指定版本
cmake_minimum_required(VERSION 3.25)

# 项目名称定义
project(
  CMakeLearn
  VERSION 1.0
)

# 生成配置文件
configure_file(
  ${PROJECT_SOURCE_DIR}/include/config/config.hpp.in
  ${PROJECT_SOURCE_DIR}/include/config/config.hpp
  @ONLY
)

# 启用测试
enable_testing()

# 设置警告标志函数
function(set_warning_flags target)
  get_target_property(target_type ${target} TYPE)
  
  if(target_type STREQUAL "INTERFACE_LIBRARY")
    set(visibility INTERFACE)
  else()
    set(visibility PRIVATE)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${target} ${visibility}
      -Wall                 # 启用所有基本警告
      -Wextra               # 启用额外警告
      -Wpedantic            # 严格遵守标准
      -Wconversion          # 可能的数值转换问题
      -Wshadow              # 变量覆盖警告
      -Wcast-align          # 指针类型转换可能导致对齐问题
      -Wcast-qual           # 指针类型转换丢失 const/volatile 限定符
      -Wunused              # 未使用的变量或函数
      -Woverloaded-virtual  # 虚函数覆盖问题
      -Wformat=2            # printf 格式字符串问题
      -Wdouble-promotion    # float 隐式转换为 double
    )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${target} ${visibility}
      -Wall                                       # 启用所有基本警告
      -Wextra                                     # 启用额外警告
      -Wpedantic                                  # 严格遵守标准
      -Wconversion                                # 可能的数值转换问题
      -Wshadow                                    # 变量覆盖警告
      -Wcast-align                                # 指针类型转换可能导致对齐问题
      -Wcast-qual                                 # 指针类型转换丢失 const/volatile 限定符
      -Wunused                                    # 未使用的变量或函数
      -Woverloaded-virtual                        # 虚函数覆盖问题
      -Wformat=2                                  # printf 格式字符串问题
      -Wdouble-promotion                          # float 隐式转换为 double
      -Wextra-semi                                # 多余的分号
      -Winconsistent-missing-destructor-override  # 析构函数override问题
    )
  endif()
endfunction()

# 设置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # 可执行文件
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # 动态库输出
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # 静态库输出

# 安装公共头文件
install(
  DIRECTORY
  ${PROJECT_SOURCE_DIR}/include
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# 包含子目录
add_subdirectory(src)