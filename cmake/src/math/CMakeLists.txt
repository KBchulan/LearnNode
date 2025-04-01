# 生成库
add_library(
  math_lib INTERFACE
)

# 包含头文件，header-only库只用包含自己即可
target_include_directories(
  math_lib INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# 增加配置选项
option(USE_MYMATH "Use our personal math implementation" ON)

# 添加预处理宏定义
if(USE_MYMATH)
  target_compile_definitions(
    math_lib INTERFACE
    USE_MYMATH
  )
endif()

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/Sqrt.hpp
  DESTINATION include/math
)