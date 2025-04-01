# 生成库文件
add_library(
  utils SHARED
  Defer.cc
)

# 包含头文件
target_include_directories(
  utils PUBLIC
  ${PROJECT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# 安装库文件
install(
  TARGETS utils
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# 安装头文件
install(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/Defer.hpp
  DESTINATION include/utils
)