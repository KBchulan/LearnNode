members = ["八奈见杏菜", "温水佳树", "小鞠知花", "白玉璃子", "志喜屋梦子", "马剃天爱星", "朝云千早"]

# if语句
if len(members) == 7:
    print("Hello, 米娜桑!")
    
# if/else语句
if members.index("温水佳树") == 0:
    print("win!")
else:
    print("failed!")

# if/elif/else语句（elif语句可以有多个）
x = complex(2, 5)
if(x.real > 6):
    print(x.real)
elif(x.real > 1 and x.imag > 6):
    print(x.imag)
else:
    print(x)
    
# 三元运算符
name = members[0] if members[0][0] == "八" else members[1]
print(name)

# match语句，相当于cc的switch
y = 10
match y:
    case 1:
        print(1)
    case 10:
        print(y)