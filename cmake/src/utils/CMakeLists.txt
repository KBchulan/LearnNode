# 引入module
include(GenerateExportHeader)

# 获取所有utils文件
file(GLOB_RECURSE SRC_UTILS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

# 生成库
add_library(
  utils
  ${SRC_UTILS_FILES}
)

# 生成导出的头文件
generate_export_header(
  utils 
  BASE_NAME UTILS
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/UtilsExport.hpp
)

# 设置位置无关
set_target_properties(
  utils PROPERTIES
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

# 指定c++23
target_compile_features(
  utils PUBLIC
  cxx_std_23
)

# 包含头文件
target_include_directories(
  utils PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

# 安装库文件和头文件
install(
  TARGETS utils
  EXPORT CMakeTemplateTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# 增加配置选项
option(USE_UTILS "Use utils" ON)
