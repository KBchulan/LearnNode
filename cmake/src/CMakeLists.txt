# 包含各个模块
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(math)

# 主程序
add_executable(${PROJECT_NAME} main.cc)

# 为Debug版本设置后缀
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

# 增加测试
add_test(NAME test1 COMMAND ${PROJECT_NAME} 25)
set_tests_properties(
  test1 PROPERTIES 
  LABELS "BasicTest"
  TIMEOUT 10
)

add_test(NAME test2 COMMAND ${PROJECT_NAME} 4)
set_tests_properties(
  test2 PROPERTIES 
  LABELS "BasicTest"
  PASS_REGULAR_EXPRESSION "mysqrt*"
  TIMEOUT 10
)

# 包含公共头文件
target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

# 动态链接子库
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  core utils
)

# 根据宏定义处理math_lib
if(USE_MYMATH)
  target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    math_lib
  )

  set_warning_flags(math_lib)
endif()

# 启动警告选项
set_warning_flags(${PROJECT_NAME})
set_warning_flags(core)
set_warning_flags(utils)

# 安装可执行文件
install(
  TARGETS
  ${PROJECT_NAME}
  DESTINATION bin
)