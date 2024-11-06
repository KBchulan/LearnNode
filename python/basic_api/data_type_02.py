# 元组，元组是不可变化的
basic_01 = (1, 2, 3.4, 5.6, "八奈见杏菜", "温水佳树", "小鞠知花", "白玉璃子", "志喜屋梦子", "马剃天爱星", "朝云千早")
basic_02 = ("a", "b", "c", "d", "e", "f", "g")

print(basic_01[4])      # 索引
print(basic_01[-1])     # 负索引

print(len(basic_01))    # 长度

mul_01 = (basic_01, basic_02)   # 嵌套元组
print(mul_01[0][5])

print(basic_01[0: 10: 2])       # 元组切片
print(basic_01[: : -1])         # 元组负切片

# 元组方法
print(basic_01.count("八奈见杏菜"))      # 记数
print(basic_01.index("八奈见杏菜"))      # 索引

# 元组解包，类似于列表初始化
a, b, c, d, e, f, g, h, i, j, k = basic_01
print(h)
begin, *other = basic_01
print(begin, other)

# 元组解包还能够交换数据
aa = 1
bb = 2
aa, bb = bb, aa
print(aa , bb) 

# 还可以用于多值返回
def min_max(values):
    return (min(values), max(values))

result = min_max((10, 20, 30, 40))
print(result)