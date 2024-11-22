import pandas as pd
import matplotlib.pyplot as plt

tips = pd.read_csv('examples/tips.csv')  
tips.head()

party_counts = pd.crosstab(tips['day'], tips['size'])
party_counts.plot(kind='bar')
plt.title('不同日期的就餐人数分布')
plt.xlabel('星期')
plt.ylabel('频次')

plt.show()