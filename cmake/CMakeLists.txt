# 这是一个比较通用的cmake配置系统，使用时请将此文件的CMakeTemplate替换为项目名称即可(请善用搜索功能)

# 指定最小版本
cmake_minimum_required(VERSION 3.25)

# 项目配置
project(
  CMakeTemplate
  VERSION 1.0.0
)

# DEBUG模式给一个'd'后缀
set(CMAKE_DEBUG_POSTFIX d)

# 静/动态库选择
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 各种情况的RunTimePath
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 生成编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 生成配置文件
configure_file(
  ${PROJECT_SOURCE_DIR}/include/config/Config.hpp.in
  ${PROJECT_SOURCE_DIR}/include/config/Config.hpp
  @ONLY
)

# 启用test功能
include(CTest)
enable_testing()

# 系统自省配置(check_cxx_source_compiles)
include(CheckCXXSourceCompiles)

# 包配置帮助器
include(CMakePackageConfigHelpers)

# 打包
include(CPack)
include(InstallRequiredSystemLibraries)

# 代码质量检测(g++, clang++)
function(set_warning_flags target)
  get_target_property(target_type ${target} TYPE)

  if(target_type STREQUAL "INTERFACE_LIBRARY")
    set(visibility INTERFACE)
  else()
    set(visibility PRIVATE)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${target} ${visibility}
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wshadow
      -Wcast-align
      -Wcast-qual
      -Wunused
      -Woverloaded-virtual
      -Wformat=2
      -Wdouble-promotion
    )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${target} ${visibility}
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wshadow
      -Wcast-align
      -Wcast-qual
      -Wunused
      -Woverloaded-virtual
      -Wformat=2
      -Wdouble-promotion
      -Wextra-semi
      -Winconsistent-missing-destructor-override
    )
  endif()
endfunction()

# 配置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# 配置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 第三方库
find_package(PkgConfig REQUIRED)
pkg_check_modules(FMT REQUIRED fmt)

# 主入口
add_subdirectory(src)

# 安装公共头文件
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# 安装导出目标
install(
  EXPORT CMakeTemplateTargets
  FILE CMakeTemplateTargets.cmake
  DESTINATION lib/cmake/CMakeTemplate
)

# 配置包文件
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_BINARY_DIR}/CMakeTemplateConfig.cmake
  INSTALL_DESTINATION lib/cmake/CMakeTemplate
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# 配置版本文件
write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/CMakeTemplateConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# 安装配置文件
install(
  FILES
  ${CMAKE_BINARY_DIR}/CMakeTemplateConfig.cmake
  ${CMAKE_BINARY_DIR}/CMakeTemplateConfigVersion.cmake
  DESTINATION lib/cmake/CMakeTemplate
)

# 打包配置
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/License.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")

# 确保编译命令文件被复制到项目根目录
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
  )
endif()
