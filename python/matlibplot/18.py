import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

comp1 = np.random.normal(0, 1, size=200)
comp2 = np.random.normal(10, 2, size=200)
values = pd.Series(np.concatenate([comp1, comp2]))
values.hist(bins=100, density=True)
values.plot(kind='kde',style='k--')
plt.show()