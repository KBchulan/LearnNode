import matplotlib.pyplot as plt

plt.text(1,1.5,'First')
plt.text(2,4.5,'Second')
plt.text(3,9.5,'Third')
plt.text(4,16.5,'Fourth')
plt.text(1.1,12,r'$y=x^2$',fontsize=28,bbox={'alpha':0.2})
plt.grid(True)

plt.show()