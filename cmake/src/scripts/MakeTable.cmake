# 生成表格的可执行文件
add_executable(MakeTable ${PROJECT_SOURCE_DIR}/src/scripts/MakeTable.cc)

# 为此文件也设置一个警告
set_warning_flags(MakeTable)

# 生成Table.hpp供数学库使用
add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/src/math/Table.hpp
  COMMAND MakeTable ${PROJECT_SOURCE_DIR}/src/math/Table.hpp
  DEPENDS MakeTable
)