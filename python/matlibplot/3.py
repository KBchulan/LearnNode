import matplotlib.pyplot as plt

plt.title("First Plot")

plt.axis([0,5,0,20])

plt.title('First Plot',fontsize=24,fontname='Times New Roman')

plt.xlabel('Counting', color='red')

plt.ylabel('Square', color='red')

plt.plot([1,2,3,4], [1,4,9,16], 'ro')

plt.xticks([0,2,4]) 

plt.show()