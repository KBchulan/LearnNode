# NumPy 基础教程

NumPy 是 Python 中用于科学计算的基础库，提供了多维数组对象和各种派生对象。以下是 NumPy 的主要功能和操作指南。

## 1. 创建数组
NumPy 提供了多种创建数组的方法，包括从列表或元组创建、使用函数创建特殊数组（如全零数组、全一数组、单位矩阵等）。
```
import numpy as np

# 从列表创建数组
arr1 = np.array([1, 2, 3, 4, 5])

# 创建特殊数组
zeros = np.zeros((3, 3))  # 3x3的零矩阵
ones = np.ones((2, 4))    # 2x4的1矩阵
empty = np.empty((2, 3))  # 2x3的未初始化矩阵
eye = np.eye(3)          # 3x3的单位矩阵
arange = np.arange(0, 10, 2)  # 创建序列：[0,2,4,6,8]
linspace = np.linspace(0, 1, 5)  # 创建等间距序列，5个数
```

## 2. 数组基本操作
了解如何查看数组的形状、大小、数据类型，以及如何进行数组的重塑和转置。
```
# 数组形状操作
arr = np.array([[1, 2, 3], [4, 5, 6]])
print(arr.shape)  # 形状
print(arr.ndim)   # 维度
print(arr.size)   # 元素总数
print(arr.dtype)  # 数据类型

# 改变形状
reshaped = arr.reshape(3, 2)  # 改变形状为3x2
flattened = arr.flatten()     # 展平为1维数组
```

## 3. 数组计算
NumPy 支持数组的基本算术运算、统计运算和线性代数运算。
```
# 基本运算
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

print(a + b)  # 加法
print(a - b)  # 减法
print(a * b)  # 乘法（元素级）
print(a / b)  # 除法
print(a ** 2) # 幂运算

# 统计运算
print(arr.sum())     # 求和
print(arr.mean())    # 平均值
print(arr.std())     # 标准差
print(arr.min())     # 最小值
print(arr.max())     # 最大值
```

## 4. 数组索引和切片
学习如何通过索引和切片来访问和修改数组中的元素。
```
arr = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])

# 基本索引
print(arr[0, 1])     # 访问单个元素
print(arr[0:2, 1:])  # 切片操作

# 布尔索引
mask = arr > 5
print(arr[mask])     # 输出大于5的元素

# 花式索引
indices = np.array([0, 2])
print(arr[indices])  # 选择特定行
```

## 5. 数组运算和函数
NumPy 提供了丰富的数学函数库，可以对数组进行各种数学运算。
```
# 数学函数
arr = np.array([1, 2, 3])
print(np.sqrt(arr))   # 平方根
print(np.exp(arr))    # 指数
print(np.log(arr))    # 对数
print(np.sin(arr))    # 三角函数

# 矩阵运算
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])

print(np.dot(a, b))   # 矩阵乘法
print(np.transpose(a))  # 转置
```

## 6. 广播机制
广播机制允许 NumPy 在不同形状的数组之间进行算术运算。
```
# 广播示例
arr = np.array([[1, 2, 3], [4, 5, 6]])
scalar = 2
print(arr * scalar)  # 广播乘法

# 不同形状数组的广播
a = np.array([[1], [2], [3]])  # 3x1
b = np.array([4, 5, 6])        # 1x3
print(a + b)  # 广播加法，结果为3x3
```

## 7. 文件操作
NumPy 支持将数组保存到文件和从文件加载数组。
```
# 保存和加载数组
arr = np.array([1, 2, 3, 4, 5])
np.save('array.npy', arr)           # 保存为二进制
np.savetxt('array.txt', arr)        # 保存为文本

loaded_arr = np.load('array.npy')   # 加载二进制
loaded_txt = np.loadtxt('array.txt')  # 加载文本
```

## 8. 随机数生成
NumPy 提供了生成随机数的功能，可以用于模拟和数据分析。
```
# 随机数操作
random_arr = np.random.rand(3, 3)    # 均匀分布
normal_arr = np.random.randn(3, 3)   # 正态分布
randint = np.random.randint(0, 10, (3, 3))  # 随机整数

# 随机种子
np.random.seed(42)  # 设置随机种子
```