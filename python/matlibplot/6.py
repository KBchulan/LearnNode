import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.add_axes([0.1, 0.1, 0.9, 0.5])
ax.plot(np.random.randn(30).cumsum(),'ko--',linewidth=4)
ax.set_title("Random")

plt.show()