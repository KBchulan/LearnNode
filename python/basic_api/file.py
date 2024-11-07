# 文件的读写各个语言其实都差不多

# 读文件
# with open("words.txt", "r") as file:
#     for index, line in enumerate(file):
#         if index <= 10:
#             print(line.strip().title())
#         else:
#             break

# 写文件
# members = {"败角": "八奈见杏菜", "妹妹": "温水佳树", "学妹": "白玉璃子", "副会长": "马剃天爱星", "传奇侦探": "朝云千早"}
# with open("memers.json", "w") as file:
#     file.write("{\n")
#     for key, value in members.items():
#         file.write('    "' + key.__str__() + '"' + ':' + '"' + value.__str__() + '"')
#         file.write(",\n")
#     file.write("}")

# 追加文件
# with open("memers.json", "a") as file:
#     file.write("{\n}")

import os
if os.path.exists("words.txt"):
    os.remove("words.txt")
else:
    print("the dir is not exists")