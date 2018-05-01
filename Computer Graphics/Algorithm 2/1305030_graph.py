import random as rn
import numpy as np
import sys


def make_cover(T, N, M):
    l=[T]
    for i in range(int(T)):
        n, m = int(N), int(M)
        l = [set([]) for i in range(m)]
        p = [i for i in range(n)]
        rn.shuffle(p)
        for i in range(n):
            q = rn.randint(1,n)
            r = np.random.choice(range(m), q)
            for j in r:
                l[j].add(p[i])
        for i in range(m):
            if l[i]==set():
                l[i]={}
        print(p)
        print(l)


def write_points(points, fname):
    sz, _ = np.shape(points)
    with open("1305030_"+fname,'w') as f:
        for i in range(sz):
            a, b = points[i]
            f.write(str(a)+" "+str(b)+'\n')
        f.close()


if __name__ == "__main__":
    # ****** show points and edges from txt file
    make_cover(sys.argv[1], sys.argv[2], sys.argv[3])
