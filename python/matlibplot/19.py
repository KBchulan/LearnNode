import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from pandas.plotting import scatter_matrix

macro = pd.read_csv('examples/macrodata.csv')
data = macro[['cpi', 'm1', 'tbilrate', 'unemp']]
trans_data = np.log(data).diff().dropna()

plt.figure(figsize=(10, 6))
plt.scatter(trans_data['m1'], trans_data['unemp'])
plt.title('M1 vs 失业率')
plt.xlabel('M1')
plt.ylabel('失业率')

fig, ax = plt.subplots(figsize=(10, 10))
scatter_matrix(trans_data, diagonal='kde', marker='*')
plt.tight_layout()

plt.show()