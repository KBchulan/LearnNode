# 包含生成导出头文件的module
include(GenerateExportHeader)

# 生成库
add_library(
  core SHARED
  ThreadPrint.cc
)

# 生成导出的头文件
generate_export_header(
  core BASE_NAME CORE
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/core_export.hpp
)

# 动态库可能被多个程序使用，因此需要位置无关声明，静态库则不需要
set_target_properties(
  core PROPERTIES
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

# 包含头文件
target_include_directories(
  core PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# 指定c++20
target_compile_features(
  core PUBLIC
  cxx_std_20
)

# 安装库文件
install(
  TARGETS core
  EXPORT CMakeLearnTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPrint.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/core_export.hpp 
  DESTINATION include/core
)