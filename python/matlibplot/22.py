import numpy as np
import matplotlib.pyplot as plt

x=np.arange(0,2*np.pi, np.pi/4)
y=np.array([4,3,7,3,5,1,2,6])
plt.axes(polar=True)
plt.bar(x,y,width=(np.pi/4),color=['#4bb2c5',
    '#c5b47f','#EAA228','#579576','#839557',
     '#2364E5','#A09874','#4b5de4'])
plt.show()