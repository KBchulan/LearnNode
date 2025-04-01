# 包含各个模块
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(math)

# 主程序
add_executable(${PROJECT_NAME} main.cc)

# 增加测试
add_test(NAME test1 COMMAND ${PROJECT_NAME} 25)

add_test(NAME test2 COMMAND ${PROJECT_NAME} 4)
set_tests_properties(
  test2 PROPERTIES PASS_REGULAR_EXPRESSION
  "mysqrt*"
)

add_test(NAME test3 COMMAND ${PROJECT_NAME} 9)
set_tests_properties(
  test3 PROPERTIES PASS_REGULAR_EXPRESSION
  "is: 3"
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