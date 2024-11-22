import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.DataFrame(np.random.rand(6, 4), index=['one', 'two', 'three', 'four', 'five', 'six'],
                  columns=pd.Index(['A', 'B', 'C', 'D'], name='Genus'))
df.plot.barh(stacked=True, alpha=0.5)
index=np.arange(4)
plt.barh(index,df.values[0],hatch='xx')
plt.barh(index,df.values[1],hatch='///',left=df.values[0])
plt.barh(index,df.values[2],hatch='\\\\\\',left=(df.values[0]+df.values[1]))
plt.barh(index,df.values[3],hatch='xx',left=(df.values[0]+df.values[1]+df.values[2]))
plt.show()
