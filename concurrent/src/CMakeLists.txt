# 各个子模块
add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(utils)

# 主程序
add_executable(${PROJECT_NAME} main.cc)
set_warning_flags(${PROJECT_NAME})

# 增加测试
add_test(
  NAME ${PROJECT_NAME}
  COMMAND ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Debug版本设置后缀
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

# 头文件
target_include_directories(
  ${PROJECT_NAME} PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# core库
if(USE_CORE)
  set_warning_flags(core)
  target_link_libraries(${PROJECT_NAME} PRIVATE core)
endif()

# math库
if(USE_MATH)
  set_warning_flags(math)
  target_link_libraries(${PROJECT_NAME} PRIVATE math)
endif()

# utils库
if(USE_UTILS)
  set_warning_flags(utils)
  target_link_libraries(${PROJECT_NAME} PRIVATE utils)
endif()

# 第三方库
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  ${FMT_LIBRARIES}
)

# 安装可执行文件
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
)