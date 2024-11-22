import numpy as np
import matplotlib.pyplot as plt

x=np.arange(-2,2,0.01)
y=np.arange(-2,2,0.01)
X,Y=np.meshgrid(x,y)
def f(x,y): return (1-y**5+x**5)*np.exp(-x**2-y**2)
plt.contourf(X,Y,f(X,Y),8,cmap=plt.cm.hot)
plt.colorbar()

C=plt.contour(X,Y,f(X,Y),8,c='black')
plt.clabel(C,inline=1,fontsize=10)
plt.show()