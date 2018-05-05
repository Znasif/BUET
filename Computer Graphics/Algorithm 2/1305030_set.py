import random as rn
import numpy as np
import sys


def make_cover(n, m):
    l = [set([]) for i in range(m)]
    p = [i for i in range(n)]
    rn.shuffle(p)
    for i in range(n):
        q = rn.randint(1,n)
        r = np.random.choice(range(m), q)
        for j in r:
            l[j].add(p[i])
    ret = []
    for i in range(m):
        if l[i] != set():
            ret.append((rn.uniform(0,2), len(l[i]), l[i]))
            # ret.append((1, len(l[i]), l[i]))
        elif l[i] == set():
            ret.append((rn.uniform(0,2), 1, [rn.randint(0, n-1)]))
            # ret.append((1, 1, [rn.randint(0, n-1)]))
    return ret



def write_points(points, fname):
    sz = len(points)
    with open("1305030_"+fname,'w') as f:
        f.write(str(sz)+'\n')
        for i in range(sz):
            a, b, c = points[i]
            f.write(a+'\n'+b+'\n')
            for j in c:
                p, q, r = j
                f.write(format(p, '.3f')+" "+str(q))
                for k in r:
                    f.write(" "+str(k))
                f.write('\n')
        f.close()


if __name__ == "__main__":
    # ****** show points and edges from txt file
    l = []
    i = int(sys.argv[1])
    while i > 0:
        j, k = input().split()
        l.append((j, k, make_cover(int(j), int(k))))
        i -= 1
    write_points(l, "out1.txt")
