from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
X, Y, Z = axes3d.get_test_data(0.05)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_wireframe(X, Y, Z, rstride=10, cstride=10)
plt.show()
