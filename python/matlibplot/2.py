import numpy as np
import matplotlib.pyplot as plt

plt.plot(np.random.randn(30).cumsum(), color='k', linestyle='dashed', marker='o')
plt.plot(np.random.randn(30).cumsum(),'ko--')

plt.show()
