import matplotlib.pyplot as plt
import numpy as np
import sys


def show_edges(points, edge=0):
    if edge == 0:
        edge = int(input("Number of edges : "))
        edges = generate_random_edges(points, edge)
    elif edge == -1:
        edge, _ = np.shape(points)
        edges = np.array([i//2 for i in range(2*edge)])
        edges = edges.reshape((edge, 2))
    else:
        edges = read_points("edges.txt")
        edge, _ = np.shape(edges)
    for a, b in points:
        plt.plot([a, a], [b, b], marker='.')
    for i in range(edge):
        a, b = edges[i]
        px, py = points[int(a)]
        qx, qy = points[int(b)]
        plt.plot([px, qx], [py, qy], marker=',')
    #plt.axis('scaled')
    plt.show()
    #write_points(edges, "edges.txt")


def take_points(vertex=0, nums=10):
    points = generate_random_vertices(nums, vertex, 0)
    write_points(points, "points.txt")
    return points


def generate_random_vertices(nums, cnt, frac=1):
    return np.random.randint(nums, size=(cnt, 2)) if frac else np.random.rand(cnt, 2)*nums


def generate_random_edges(points, cnt):
    sz, _ = np.shape(points)
    return generate_random_vertices(sz, cnt)


def read_points(fname):
    with open("1305030_"+fname, 'r') as f:
        lines = f.readlines()
        points = []
        flag = 1
        for l in lines:
            if flag:
                flag = 0
                continue
            a, b = l.split()
            if fname == "points.txt":
                points.append((float(a), float(b)))
            else:
                points.append((int(a), int(b)))
        points = np.array(points)
        f.close()
    return points


def write_points(points, fname):
    sz, _ = np.shape(points)
    with open("1305030_"+fname, 'w') as f:
        if fname == "points.txt":
            f.write(str(sz)+'\n')
        for i in range(sz):
            a, b = points[i]
            f.write(f'{a:.3f}'+" "+f'{b:.3f}'+'\n')
        r = (int(sys.argv[2])//2)
        queries = generate_random_vertices(10, r*3, 0)
        i = 0
        f.write(str(r*2)+'\n')
        r *= 3
        while(i<r):
            if(i%3==0):
                a, b = queries[i]
                c, d = queries[i+1]
                f.write("R "+f'{a:.3f}'+" "+f'{b:.3f}'+" "+f'{c:.3f}'+" "+f'{d:.3f}'+'\n')
                i+=2
            else:
                a, b = queries[i]
                f.write("N "+f'{a:.3f}'+" "+f'{b:.3f}'+'\n')
                i+=1
        f.close()


if __name__ == "__main__":
    # ****** show randomly generated points and edges and store them in txt file
    vertices = take_points(int(sys.argv[1]))
    show_edges(vertices, -1)
