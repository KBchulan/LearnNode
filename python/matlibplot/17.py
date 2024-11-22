import pandas as pd
import matplotlib.pyplot as plt

tips = pd.read_csv('examples/tips.csv')
tips['tip_pct'] = tips['tip'] / (tips['total_bill'] - tips['tip'])

plt.figure(figsize=(10, 6))

tips['tip_pct'].plot.hist(bins=50, alpha=0.5, label='直方图')

tips['tip_pct'].plot.density(color='red', label='密度曲线')

plt.title('小费百分比分布')
plt.xlabel('小费百分比')
plt.ylabel('频次')
plt.legend()

plt.show()