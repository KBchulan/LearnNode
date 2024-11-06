members = ["八奈见杏菜", "温水佳树", "小鞠知花", "白玉璃子", "志喜屋梦子", "马剃天爱星", "朝云千早"]
tuple = ("a", "b", "c", "d", "e", "f", "g")
directory = {"败角": "八奈见杏菜", "妹妹": "温水佳树", "学妹": "白玉璃子", "副会长": "马剃天爱星", "传奇侦探": "tmp"}

# for循环
'''
for <loop_variable> in <iterable>:
    <code>
'''
# 其中的iterator是一个可迭代对象可以是一个列表，元组，字典，字符串，range 返回的一个序列，一个文件，或者其他任意的可迭代类型的数据

# 看一下源码可以知道有三个参数start，stop(!=)，step
# 传参的顺序是stop，start，step(其余采用默认实参)
for member in range(6, 0, -1):
    print(member)
    
print("-------------------")

# str
message = "hello,str"
for char in message.title():
    print(char)
    
print("-------------------")

# list and tuple
for member in members:
    print(member)

for tup in tuple:
    if tup < "d":
        print(tup)
    
print("-------------------")

# 字典
for key in directory.keys():
    print(key)
    
for value in directory.values():
    print(value)

# 下面这个出来的是元组，我们也可以看出多个值的返回我们会采用元组
for item in directory.items():
    print(item)

for key, value in directory.items():
    print(key + ' ' + value)
    
print("-------------------")
    
# 循环控制语句：break和continue，非常基础，此处跳过

# 我们介绍一下zip函数
for elem1, elem2 in zip(members, tuple):
    print(elem2 + ': ' + elem1)

print("-------------------")

# 我们再介绍一下enumerate函数
for i, elem in enumerate(members):
    print(i.__str__() + ': ' + elem)

print("-------------------")

# 我们来聊一下else语句，有个认知即可，如果for循环没有执行break语句，则最后的else才会被执行

# while语句此处与for的区别与cc里相同，不再赘述

