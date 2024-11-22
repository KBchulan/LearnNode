import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

fig = plt.figure()
line1 = Line2D([0,1],[0,1], transform=fig.transFigure, figure=fig, color="r")
line2 = Line2D([0,1],[1,0], transform=fig.transFigure, figure=fig, color="g")
fig.lines.extend([line1, line2])

plt.show()