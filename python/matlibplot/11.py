import numpy as np
import matplotlib
import matplotlib.pyplot as plt

plt.rc('figure',figsize=(0.5,1))
font_options={'weight':'bold', 'size':'24'}
plt.rc('font', **font_options)
plt.plot(np.random.randn(30).cumsum(),'ko--',linewidth=2)
plt.title('random')
matplotlib.rcdefaults()

plt.show()