import numpy as np
import sys


if __name__ == "__main__":
    n = int(sys.argv[1])
    m = int(sys.argv[2])
    c = np.random.randint(ord('a'), ord('e')+1, n)
    b = np.random.randint(ord('a'), ord('e')+1, m)
    T = ""
    P = ""
    for i in range(n):
        T += chr(c[i])
    for i in range(m):
        P += chr(b[i])
    with open("1305030_string.txt", 'w') as f:
        f.write(sys.argv[1]+"\n"+T+'\n'+sys.argv[2]+'\n'+P)
        f.close()
