# 列表和字典推导，在前面的函数文件中我们其实已经有使用过，这里单独拿出来说说
"""
[<value_to_include> for <var> in <sequence>]
[<value_to_include> for <var1> in <sequence1> for <var2> in <sequence2>]
[<value_to_include> for <var> in <sequence> if <condition>]
[<value> for <var1> in <sequence1> for <var2> in <sequence2> if <condition>]
"""

print([i for i in range(4, 15)])

print([chr(i) for i in range(67, 80)])

print([i**3 for i in range(1, 5)])

print([i + j for i in range(5, 8) for j in range(3, 6)])

print([k for k in range(1, 20) if k % 2 == 0])

# 生成器表达式
# 这是一种懒加载的方式，因此占用的存储是比较低的
