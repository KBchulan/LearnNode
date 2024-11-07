# py的异常处理

# 以下是错误类型

# zeroDivisionError
print(5 / 0)

# IndexError
my_list = [1, 2, 3]
print(my_list[5])

# KeyError, if you use get, it will return None
my_dir = {1: "a", 2: "b"}
print(my_dir[3])

# NameError, not define error
print(b)

# RecursionError, it has no end
def error(n):
    if n <= 2:
        return 1
    return n * error(n)

error(3)

# 下面说一下异常处理，即我们的try/except
"""
try:
    <code_that_may_raise_an_exception>
except:
    <code_to_handle_the_exception_if_it_occurs>
如果try模块的内容出现异常，则except才会执行
"""

a = int(input())
b = int(input())
try:
    print(a / b)
except:
    print("please enter valid values")

# 还可以捕获特定异常
try:
    print(a / b)
except ZeroDivisionError:
    print("please enter valid values")


# 还可以为异常信息指定名字
try:
    print(a / b)
except ZeroDivisionError as error:
    print("error is: ", error)

# 若是try模块没有发生异常，我们可以用else模块捕获
try:
    print(a / b)
except ZeroDivisionError as error:
    print("error is: ", error)
else:
    print("ok")

# 还有一个finally，无论执行有无异常，执行完try都会进行的模块
try:
    print(a / b)
except ZeroDivisionError as error:
    print("error is: ", error)
else:
    print("ok")
finally:
    print("end")
# 最后这个可以在有异常时关闭文件
