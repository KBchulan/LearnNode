# 生成导出文件的module
include(GenerateExportHeader)

# 生成库文件
add_library(
  utils SHARED
  Defer.cc
)

# 生成导出头文件
generate_export_header(
  utils BASE_NAME UTILS
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/utils_export.hpp
)

# 设置动态库位置无关
set_target_properties(
  utils PROPERTIES
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

# 包含头文件
target_include_directories(
  utils PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# 安装库文件
install(
  TARGETS utils
  EXPORT CMakeLearnTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/Defer.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utils_export.hpp 
  DESTINATION include/utils
)