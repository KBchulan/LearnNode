# 生成库
add_library(math INTERFACE)

# 指定c++23
target_compile_features(
  math INTERFACE
  cxx_std_23
)

# 包含头文件
target_include_directories(
  math INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# 安装库文件和头文件
install(
  TARGETS math
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
option(USE_MATH "Use math" ON)
