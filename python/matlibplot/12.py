import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0.0, 2.0*np.pi, 0.01) 
s = np.sin(t)
z = np.cos(t)
plt.plot(t, s, label='sin')
plt.plot(t, z, label='cos')
plt.xlabel('x-变量', fontproperties='SimHei', fontsize=24)
plt.ylabel('y-正弦余弦函数值', fontproperties='SimHei', fontsize=24)
plt.title('sin-cos函数图像', fontproperties='SimHei', fontsize=32)
plt.legend('best') 
plt.show()