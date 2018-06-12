import numpy as np

n = input()
n = int(n)

a = np.random.randint(0, 2*n, n)
c = np.random.randint(0, 3, n)

a = np.append(n, a)
c = np.append(n, c)

np.savetxt('1305030_input.txt', np.c_[c, a], fmt='%d')
