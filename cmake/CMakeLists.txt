# https://cmake.org/cmake/help/latest/guide/tutorial/index.html
# 正常make后现在支持的有：
# ctest 
# ctest -D Experimental(CDash)
# custom command
# cpack
# make install

# 指定版本
cmake_minimum_required(VERSION 3.25)

# 项目名称定义
project(
  CMakeLearn
  VERSION 1.0
)

# 为Debug版本的库设置'd'后缀
set(CMAKE_DEBUG_POSTFIX d)

# 设置静态库和动态库的选择
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# 设置RPATH(给打包以后的库设置RPATH)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 生成配置文件
configure_file(
  ${PROJECT_SOURCE_DIR}/include/config/config.hpp.in
  ${PROJECT_SOURCE_DIR}/include/config/config.hpp
  @ONLY
)

# 启用测试
include(CTest)
enable_testing()

# 增加系统自省功能
include(CheckCXXSourceCompiles)

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

# 安装导出目标
install(
  EXPORT CMakeLearnTargets
  FILE CMakeLearnTargets.cmake
  DESTINATION lib/cmake/CMakeLearn
)

# 包配置帮助器
include(CMakePackageConfigHelpers)

# 配置包文件
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeLearnConfig.cmake
  INSTALL_DESTINATION lib/cmake/CMakeLearn
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# 写入版本文件
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CMakeLearnConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

# 安装配置文件
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeLearnConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeLearnConfigVersion.cmake
  DESTINATION lib/cmake/CMakeLearn
)

# 打包配置
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")        # 可以打包为deb格式的
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)