import numpy as np

a = np.array(
    [20, 30, 40, 50]
)

b = np.arange(4)

c = a - b

print(c)

print(b**2)

print(a < 35)

# 矩阵相乘
A = np.array([
    [1, 1],
    [0, 1]
])

B = np.array([
    [2, 0],
    [3, 4]
])

print(A @ B)

rg = np.random.default_rng(1)

c = np.ones(
    (2, 3), dtype = int
)
d = rg.random(2, 3)

c *= 3
print(c)

d += c
print(d)