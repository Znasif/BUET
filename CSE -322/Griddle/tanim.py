import numpy as np
import matplotlib.pyplot as plt
import math

n = int(input("The sides of the star :"))
r  = int(input("Distance from center to a corner : "))

x, y, next_x, next_y = [], [], [], []
theta = 2*math.pi/n
for i in range(n):
    x.append((r+r)*math.sin(i*theta))
    next_x.append(r*math.sin(theta/2+i*theta))
    y.append((r+r)*math.cos(i*theta))
    next_y.append(r*math.cos(theta/2+i*theta))

for i in range(n):
    plt.plot([x[i], next_x[i]], [y[i], next_y[i]], marker='.')
for i in range(n-1):
    plt.plot([x[i+1], next_x[i]], [y[i+1], next_y[i]], marker='.')
plt.plot([x[0], next_x[-1]], [y[0], next_y[-1]], marker='.')

plt.axis('scaled')
plt.show()

#print((((3)**.5)*(a)**2)*b)
