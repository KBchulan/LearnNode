import numpy as np
import matplotlib.pyplot as plt

plt.plot(np.random.randn(30).cumsum(), color='k', linestyle='dashed', marker='o',label='one')
plt.plot(np.random.randn(30).cumsum(), color='r', linestyle='dashed', marker='o',label='two')

plt.legend(loc='lower left')
plt.savefig('figpath.png', dpi=400, bbox_inches='tight')

plt.show()