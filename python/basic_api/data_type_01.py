# 列表
# 下面是基础的列表,可以有不同类型，跟我们cc的variant以及tuple一样
basic_1 = [1, 2, 3.4, 5.6, "八奈见杏菜", "温水佳树", "小鞠知花", "白玉璃子", "志喜屋梦子", "马剃天爱星", "朝云千早"]
basic_2 = ["a", "b", "c", "d", "e", "f", "g"]
# 列表嵌套
mul_1 = [basic_1, basic_2]
print(basic_1, basic_2, mul_1)

# 长度
print(len(mul_1))

# 列表索引，[start = 0 : < (stop = last_idx) : step = 1]
print(basic_2[1])           # 索引访问
print(basic_2[0: 6: 2])     # 切片
print(basic_2[: : -1])      # 默认值切片
basic_2[2] = "d"
print(basic_2)
basic_2[2] = "c"

# 列表方法
basic_2.append("h")          # 将一个元素添加到末尾(直接修改原来的，不返回新的)
print(basic_2)
basic_2.extend(["i", "k"])   # 将一个列表添加到末尾(直接修改原来的，不返回新的)
print(basic_2)
basic_2.insert(0, "o")       # 在指定索引处插入元素
print(basic_2)
basic_2.remove("o")          # 删除第一个出现的元素
print(basic_2)
basic_2.pop()                # 移除指定位置的元素，默认最后一个
print(basic_2)

basic_2.sort(reverse = True)          # 排序（默认升序）
print(basic_2)
basic_2.sort()
print(basic_2)
# 此外还有clear（清除列表），index（返回元素索引），count（元素出现次数）