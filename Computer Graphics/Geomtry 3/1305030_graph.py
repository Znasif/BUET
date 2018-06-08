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
    write_points(edges, "edges.txt")


def take_points(nums=5, vertex=0):
    if vertex == 0:
        vertex = int(input("Number of vertices : "))
        points = generate_random_vertices(nums, vertex, 0)
        write_points(points, "points.txt")
        return points
    return read_points("points.txt")


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
            f.write(str(a)+" "+str(b)+'\n')
        f.close()


if __name__ == "__main__":
    # ****** show points and edges from txt file
    if(sys.argv[1]=="1"):
        vertices = take_points(5, 1)
        show_edges(vertices, 1)
    elif(sys.argv[1]=="2"):
        # ****** show randomly generated points and edges and store them in txt file
        vertices = take_points()
        show_edges(vertices, 1)
    else:
        vertices = read_points(sys.argv[1])
        show_edges(vertices, -1)
