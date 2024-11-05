# int
print(type(18))

# 浮点数
print(type(4.5))

# 复数
print(complex(6, 8))

# 字符串：''或者""，保持统一即可，外内不一样即可混用
print("The girl's name is jiashu and she is cute")

# 索引，0->...  || -1->...
name = "八奈见杏菜"
print(name[1])
print(name[-2])

# 字符串切片
# 如果不指定则会选择默认值
# <name>[start = 0 : < (stop = last_idx) : step = 1]
print(name[1: 3])
print(name[1: 6])
print(name[0: 5: 2])
print(name[ : : 2])
print(name[ : : -1])

# f-str，执行字符串替换，同时还能调用方法
text = "I like jiashu!"
print(f"just a test:{text}...")
print(f"just a test:{text.upper()}...")

# 字符串方法
# 基本操作
print(text.capitalize())    # 第一个字符成大写
print(text.casefold())      # 全部转换为小写（更为宽泛，可以处理unicode字符）
print(text.lower())         # 全部转换为小写
print(text.upper())         # 全部变为大写
print(text.title())         # 每个单词首字母大写
print(text.swapcase())      # 大写变小写，小写变大写
print(text.strip())         # 删除开头和结尾的空白（或者指定的）
print(text.lstrip())        # 同上，只有开头的
print(text.rstrip())        # 同上，只有结尾的
# 查找和替换
print(text.count('i'))      # 计数
print(text.find('s'))       # 返回第一次出现的索引（没找到返回-1,不会抛出异常）
print(text.rfind('s'))      # 返回最后一次出现的索引（没找到返回-1,不会抛出异常）
print(text.index('s'))      # 返回第一次出现的索引（没找到抛出异常）
print(text.rindex('s'))     # 返回最后一次出现的索引（没找到抛出异常）
print(text.replace("jiashu", "xingcai"))    # old(idx = 1)->new(idx = 2)
# 测试和检查
print(text.startswith('I')) # 以参数开头
print(text.endswith('!'))   # 以参数结尾
print(text.isdigit())       # 只含数字
print(text.isalpha())       # 只含字母
print(text.isalnum())       # 只含数字或字母
print(text.isspace())       # 只含空白
print(text.islower())       # 全是小写
print(text.isupper())       # 全是大写
# 格式化字符串
print("hello, {}".format("杏菜"))   # 插入{}
print(text.zfill(20))       # 用0从前面填充至指定长度
print(text.center(20, '-')) # 用指定字符填充至中间
print(text.ljust(20, '-'))  # 字符串左对齐
print(text.rjust(20, '-'))  # 字符串右对齐
# 分割和连接
print(text.split())         # 按照指定字符分割（默认空白）
print(text.rsplit())        # 从右侧按照指定字符分割（默认空白）
print("jiashu\nmeimei".splitlines())    # 以换行符号分割
print(text.join(['a', 'b']))            # 以字符串做连接桥

# 布尔类型