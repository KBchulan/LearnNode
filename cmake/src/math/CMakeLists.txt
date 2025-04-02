# 加载脚本
include(${PROJECT_SOURCE_DIR}/src/scripts/MakeTable.cmake)

# 自定义一个target用于给math_lib增加依赖
add_custom_target(
  generate_table_header
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Table.hpp
)

# 生成库
add_library(
  math_lib INTERFACE
)

# 增加依赖
add_dependencies(
  math_lib generate_table_header
)

# 包含头文件，header-only库只用包含自己即可
target_include_directories(
  math_lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# 指定c++20
target_compile_features(
  math_lib INTERFACE
  cxx_std_20
)

# 增加配置选项
option(USE_MYMATH "Use our personal math implementation" ON)

# 添加预处理宏定义(option增加)
if(USE_MYMATH)
  target_compile_definitions(
    math_lib INTERFACE
    USE_MYMATH
  )
endif()

# 系统自省定义宏
check_cxx_source_compiles(
  "#include <cmath>
  
  int main()
  {
    std::log(1.0);
    return 0;
  }
  " HAVE_LOG
)

# 添加预处理宏定义(系统自省增加)
if(HAVE_LOG)
  target_compile_definitions(
    math_lib INTERFACE
    HAVE_LOG
  )
endif()

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/Sqrt.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Table.hpp
  DESTINATION include/math
)