import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_axes([0.1, 0.45, 0.8, 0.5])
ax2 = fig.add_axes([0.1, 0.1, 0.8, 0.2])

fig = plt.figure()
ax1 = fig.add_subplot(2,1,1)
ax2 =fig.add_subplot(2,1,2)

plt.subplot(211)
plt.subplot(212)

plt.show()