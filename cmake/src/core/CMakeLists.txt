# 生成库
add_library(
  core SHARED
  ThreadPrint.cc
)

# 包含头文件
target_include_directories(
  core PUBLIC
  ${PROJECT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# 指定c++20
target_compile_features(
  core PUBLIC
  cxx_std_20
)

# 安装库文件
install(
  TARGETS core
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPrint.hpp
  DESTINATION include/core
)