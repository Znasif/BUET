import numpy as np
import sys

if __name__ == "__main__":
    n = int(sys.argv[2])
    if(sys.argv[1] == "0"):
        a = np.random.randint(0, 2*n, n)
        c = np.random.randint(0, 3, n)
    elif(sys.argv[1] == "1"):
        a = np.array([(2*i)//n for i in range(n)])
        c = np.array([0 for i in range(n)])
    else:
        a = np.array([n for i in range(n)])
        c = np.array([0 for i in range(n)])
    a = np.append(n, a)
    c = np.append(n, c)
    np.savetxt('1305030_input.txt', np.c_[c, a], fmt='%d')
