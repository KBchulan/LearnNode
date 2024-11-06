# 字典，就是我们的键值对
basic_01 = {"败角": "八奈见杏菜", "妹妹": "温水佳树", "学妹": "白玉璃子", "副会长": "马剃天爱星", "传奇侦探": "tmp"}
# 字典的键是一种可变类型（不能是列表）
# 字典的值可以是任何类型（包括列表和字典）

print(len(basic_01))        # 获取长度

basic_01["传奇侦探"] = "朝云千早"      # 通过键修改值
print(basic_01)

basic_01["隐形系"] = "小鞠知花"        # 增加键值对
print(basic_01)

del basic_01["隐形系"]                # 删除键值对
print(basic_01)

# 字典的方法（清空的函数不做演示）
print(basic_01["败角"])        # 通过键取出来值，不存在会抛出key_error异常，python爷爷用object打下TypeError
print(basic_01.get("妹妹"))    # 通过get取出来值，可以指定没找到的默认值
print(basic_01.keys())        # 获取所有的键
print(basic_01.values())      # 获取所有的值
print(basic_01.items())       # 获取所有的键值对

basic_01_copy = basic_01.copy() # 进行浅拷贝
print(basic_01_copy)

basic_02 = {"学姐": "志喜屋梦子"}
basic_01.update(basic_02)
print(basic_01)