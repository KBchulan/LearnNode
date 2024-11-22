import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

def randrange(n, vmin, vmax):
    '''生成n个在[vmin, vmax]范围内均匀分布的随机数'''
    return (vmax - vmin) * np.random.rand(n) + vmin 

n = 100

for c, m, zlow, zhigh in [('r', 'o', -50, -25), ('b', '^', -30, -5)]:
    xs = randrange(n, 23, 32)
    ys = randrange(n, 0, 100)
    zs = randrange(n, zlow, zhigh)
    ax.scatter(xs, ys, zs, c=c, marker=m, label=f'{c}组 ({m})')

ax.set_xlabel('X 轴')
ax.set_ylabel('Y 轴')
ax.set_zorder('Z 轴')
ax.set_title('3D散点图示例')
ax.legend()

plt.show()
