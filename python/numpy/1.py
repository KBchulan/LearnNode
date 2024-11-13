import numpy as np

# 数组创建（差不多是矩阵）

# 最基础的创建
print("----------------------------------")
print(
    np.array([2, 3, 4]).dtype
)

# 按照高精度补全
print("----------------------------------")
print(
    np.array([3, 2, 61.2, 99.3]).dtype
)

# 显示指定类型
print("----------------------------------")
c = np.array([
   [3.2, 61.2, 99.3],
   [2, 6, 7]
], dtype = float)
print(c)


# 特殊初始化: zeros, ones, empty(未初始化), eye(单位矩阵)
print("----------------------------------")
print(
    np.zeros((3, 4), dtype = np.int64)
)

print(
    np.ones((3, 4), dtype = np.int64)
)

print(
    np.empty((3, 4), dtype = np.int64)
)

print(
    np.eye((3))
)

# 数字序列: 普通arange， 等间距序列linspace
print("----------------------------------")
print(
    np.arange(0, 10, 1)
)

print(
    np.linspace(0, 2, 10)
)

print("----------------------------------")
