# 函数这个东西，我觉得在我们cc学习以后，应该是足够熟悉的，直接展示例子来吧

# 简单的加法
def add(a, b):
    return a + b

print(add(5, 6))

# 默认参数
def greet(name = "八奈见杏菜"):
    print(f"hello, {name}, have you eated?")

greet()

# 关键字作为参数(py也是有重载的)
def greet(name, age):
    print(f"Hello, {name}! You are {age} years old.")

greet(age = 20, name = "八奈见杏菜")

# 参数包(cc17引入):*args(任意位置参数),**kwargs(任意关键字参数)
def print_args(*args):
    for arg in args:
        print(arg)

print_args(1, "aa", 3.4, [1, 2, 3], (4, 5, 6), {7: 8})

def print_kwargs(**kwargs):
    for item in kwargs.items():
        print(item)

print_kwargs(name = "sam", age = "5")

# 多个值的返回，感觉没必要说

# 经典函数的实现
# 阶乘
def factorial(n):
    if n == 1:
        return 1
    return n * factorial(n - 1)

print(factorial(3))

# 斐波那契数列
def fib(n):
    if n <= 2:
        return 1
    return fib(n - 1) + fib(n - 2)

print(fib(6))

# 冒泡排序
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if(arr[j] > arr[j + 1]):
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

print(bubble_sort([64, 34, 25, 12, 22, 11, 90]))

# 快排
def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    
    pivot = arr[0]
    left = [x for x in arr[1: ] if x < pivot]
    right = [x for x in arr[1: ] if x >= pivot]
    return quick_sort(left) + [pivot] + quick_sort(right)

print(quick_sort([64, 34, 25, 12, 22, 11, 90]))

# 生成器，生成素数，yield是一个阻塞式的函数
def generate_primes():
    num = 2
    while True:
        for i in range(2, num):
            if num % i == 0:
                break
        else:
            yield num
        num += 1

prime = generate_primes()
for _ in range(5):
    print(next(prime))