import copy as cp
import time
import random

def print_(self):
    m=["Teacher : ","Class : ","Room : "]
    for i in self.l:
        for j in i:
            for k in range(3):
                print(m[k]+str(j[k]+1), end=" ")
            print(end="\t")
        print()

def swap(l, i, j, i_, j_):
    l[i][j], l[i_][j_] = l[i_][j_], l[i][j]

def cost(l,w=[1, 1, 1]):
    p, q = len(l), len(l[0])
    # p,q,_=self.l.shape
    cost_ = [0, 0, 0]
    for i in range(p):
        for j in range(q - 1):
            for k in range(j + 1, q):
                for a in range(3):
                    if l[i][j][a] == l[i][k][a]:
                        cost_[a] += w[a]
    a = 0
    for i in range(3):
        a += cost_[i]
    return a


def hillClimb(R, w):
    cost_ = cost(R,w)
    if (cost_ == 0): return cost_
    p, q = p, q = len(R), len(R[0])
    # p,q,_=R.l.shape
    #t1.l = np.array(R.l)
    t1=cp.deepcopy(R)
    while (1):
        flag = 0
        for i in range(p - 1):
            for j in range(p):
                for k in range(q):
                    for l in range(q):
                        swap(t1,i, k, j, l)
                        c = cost(t1,w)
                        if (c < cost_):
                            cost_ = c
                            if (cost_ == 0): return cost_
                            #R.l = np.array(t1.l)
                            R=cp.deepcopy(t1)
                            flag = 1
                        swap(t1,i, k, j, l)
        if (flag == 0):
            return cost_
        #t1.l = np.array(R.l)
        t1=cp.deepcopy(R)


def store(fname):
    info = []

    with open(fname[0]) as f:
        lines = f.readlines()
        for l in lines:
            s = l.split()
            info.append(int(s[-1]))
        f.close()

    info.pop(1)
    w = []
    periods = 30

    with open(fname[1]) as f:
        lines = f.readlines()
        p = lines[0].split()
        periods = int(p[2]) * int(p[6])
        for l in lines[-9:-6:1]:
            s = l.split()
            w.append(int(s[-1]))
        f.close()

    R = [[] for i in range(info[2])]

    with open(fname[2]) as f:
        lines = f.readlines()
        cnt = 0
        for l in lines:
            s = l.split()
            for i in range(info[0]):
                for j in range(int(s[i])):
                    R[cnt // info[2]].append((i, cnt % info[2], cnt // info[2]))
            cnt += 1
        f.close()


    Requirement = [[[] for j in range(info[2])] for i in range(periods)]
    for i in range(periods):
        for j in range(info[2]):
            Requirement[i][j] = R[j].pop(random.randint(0,len(R[j])-1))

    return Requirement, w

def dim(a):
    if not type(a) == list:
        return []
    return [len(a)] + dim(a[0])

if __name__ == "__main__":
    start_time = time.time()
    a = "hdtt"
    b = ["note.txt", "list.txt", "req.txt"]
    c = input("Select input File : ")
    R, w = store([a + c + b[0], a + c + b[1], a + c + b[2]])
    print("Weights : " + str(w))
    print(dim(R))
    #R.print_()
    #cost_ = hillClimb(R, w)
    #print("Cost : " + str(cost_))
    print("--- %s seconds ---" % (time.time() - start_time))
    a = input("Press \"Enter\" to Terminate")
