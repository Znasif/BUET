import heapq as hq


class Grid:
    def set(self, n, l, t):
        self.n = n
        self.l = []
        self.t = []
        for i in range(n):
            tmp = []
            for j in range(n):
                tmp.append(l[i][j])
            self.l.append(tmp)
        for i in range(n):
            tmp = []
            for j in range(n):
                tmp.append(t[i][j])
            self.t.append(tmp)

    def is_goal(self):
        target=[]
        for i in range(4):
            self.t = list(zip(*self.t[::-1]))
            l=[]
            for j in self.t:
                temp=[]
                for a in j:
                    temp.append(a)
                l.append(temp)
            target.append(l)
        if self.l in target:
            return True
        return False

    def slide(self, num, x):
        if(num==0): self.slide_up(x)
        if(num==1): self.slide_down(x)
        if(num==2): self.slide_left(x)
        if(num==3): self.slide_right(x)

    def slide_up(self, col):
        tmp = self.l[0][col];
        for i in range(1, self.n):
            self.l[i - 1][col] = self.l[i][col]
        self.l[self.n - 1][col] = tmp;

    def slide_down(self, col):
        tmp = self.l[self.n - 1][col];
        for i in range(self.n - 2, -1, -1):
            self.l[i + 1][col] = self.l[i][col]
        self.l[0][col] = tmp;

    def slide_right(self, row):
        tmp = self.l[row][self.n - 1]
        for i in range(self.n - 2, -1, -1):
            self.l[row][i + 1] = self.l[row][i]
        self.l[row][0] = tmp

    def slide_left(self, row):
        tmp = self.l[row][0]
        for i in range(1, self.n):
            self.l[row][i - 1] = self.l[row][i]
        self.l[row][self.n - 1] = tmp

    def heuristic_cost(self):
        cost = 0
        target=[]
        for i in range(4):
            self.t = list(zip(*self.t[::-1]))
            l=[]
            for j in self.t:
                temp=[]
                for a in j:
                    temp.append(a)
                l.append(temp)
            target.append(l)
        for i in range(self.n):
            for j in range(self.n):
                if self.l[i][j] != self.t[i][j]: cost += 1
        return cost


def a_star(G):
    pq = []
    h = G.heuristic_cost()
    g, visited, parent = {}, {}, {}
    g[G] = 0
    visited[G] = 1
    if (G.is_goal()): return g, visited, parent
    hq.heappush(pq, (h, G))
    while(len(pq) != 0):
        nu, temp = hq.heappop(pq)
        for i in range(temp.n):
            for j in range(4):
                state = temp
                state.slide(j,i)
                if(visited[state]!=1):
                    visited[state] = 1
                    g[state] = g[temp]+1
                    parent[state] = (i,temp)
                    if(state.is_goal()): return g, visited, parent
                    hq.heappush(pq, (g[state]+state.heuristic_cost(), state))
    return  g, visited, parent

def get_input(fname):
    with open(fname) as f:
        s= []
        lines = f.readlines()
        for i in lines:
            for j in i:
                if(j=='\n' or j==' '): continue
                s.append(j)
        f.close()

    n=int(s[0])
    k = 1
    l,t=[],[]
    for i in range(n):
        temp=[]
        for j in range(n):
            temp.append(int(s[k]))
            k+=1
        l.append(temp)
    for i in range(n):
        temp=[]
        for j in range(n):
            temp.append(int(s[k]))
            k+=1
        t.append(temp)
    return n, l, t


def Main():
    fname = "iput.txt"
    n,l,t = get_input(fname)
    G = Grid(n,l,t)
    g, visited, parent = a_star(G)
    if(visited[t]==0): print("Impossible")
    #else:
        #if(g[t]):


if __name__ == "__main__":
    Main ()