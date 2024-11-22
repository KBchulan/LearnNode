import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(2,1,1)
ax2 =fig.add_subplot(2,1,2)
x1 = np.linspace(0.0, 5.0)
x2 = np.linspace(0.0, 3.0)
y1 = np.cos(2 * np.pi * x1) * np.exp(-x1)
y2 = np.cos(2 * np.pi * x2)
ax1.patch.set_facecolor("green")
ax1.grid(True)
line1 = ax1.plot(x1, y1, 'yo-', label="Test1")
line2 = ax2.plot(x2, y2, 'r.-', label='Test2')

plt.show()