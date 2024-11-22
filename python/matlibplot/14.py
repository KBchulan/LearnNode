import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
fig, axes = plt.subplots(2, 1)
data = pd.Series(np.random.rand(16), index=list('abcdefghijklmnop'))
data.plot.bar(ax=axes[0], color='k', alpha=0.7) #alpha:点的不透明度
data.plot.barh(ax=axes[1], color='k', alpha=0.7)
np.random.seed(12348)
df = pd.DataFrame(np.random.rand(6, 4), index=['one', 'two', 'three', 'four', 'five', 'six'],
                  columns=pd.Index(['A', 'B', 'C', 'D'], name='Genus'))
df.plot.bar()
plt.show()
