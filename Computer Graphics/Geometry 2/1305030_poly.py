import math, random
import matplotlib.pyplot as plt
import sys

def generatePolygon( ctrX, ctrY, aveRadius, irregularity, spikeyness, numVerts ) :
    '''Start with the centre of the polygon at ctrX, ctrY,
        then creates the polygon by sampling points on a circle around the centre.
        Randon noise is added by varying the angular spacing between sequential points,
        and by varying the radial distance of each point from the centre.

        Params:
        ctrX, ctrY - coordinates of the "centre" of the polygon
        aveRadius - in px, the average radius of this polygon, this roughly controls how large the polygon is, really only useful for order of magnitude.
        irregularity - [0,1] indicating how much variance there is in the angular spacing of vertices. [0,1] will map to [0, 2pi/numberOfVerts]
        spikeyness - [0,1] indicating how much variance there is in each vertex from the circle of radius aveRadius. [0,1] will map to [0, aveRadius]
        numVerts - self-explanatory

        Returns a list of vertices, in CCW order.
    '''
    irregularity = clip( irregularity, 0,1 ) * 2*math.pi / numVerts
    spikeyness = clip( spikeyness, 0,1 ) * aveRadius

    # generate n angle steps
    angleSteps = []
    lower = (2*math.pi / numVerts) - irregularity
    upper = (2*math.pi / numVerts) + irregularity
    sum = 0
    for i in range(numVerts) :
        tmp = random.uniform(lower, upper)
        angleSteps.append( tmp )
        sum = sum + tmp

    # normalize the steps so that point 0 and point n+1 are the same
    k = sum / (2*math.pi)
    for i in range(numVerts) :
        angleSteps[i] = angleSteps[i] / k

    # now generate the points
    points = []
    angle = random.uniform(0, 2*math.pi)
    for i in range(numVerts) :
        r_i = clip( random.gauss(aveRadius, spikeyness), 0, 2*aveRadius )
        x = ctrX + r_i*math.cos(angle)
        y = ctrY + r_i*math.sin(angle)
        points.append( (int(x),int(y)) )
        angle = angle + angleSteps[i]

    return points

def write_points(points, fname):
    sz = len(points)
    with open("1305030_"+fname,'w+') as f:
        f.write(str(sz)+'\n')
        for i in range(sz):
            a, b = points[i]
            if i != sz-1:
                x, y = points[i+1]
                plt.plot([a, x], [b, y], marker='o')
            else:
                x, y = points[0]
                plt.plot([a, x], [b, y], marker='o')
            f.write(str(a)+" "+str(b)+'\n')
        plt.axis('scaled')
        plt.show()
        f.close()


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
            points.append((float(a), float(b)))
        f.close()
    #print(points)
    return write_points(points, fname)

def clip(x, min, max) :
    if( min > max ) :  return x
    elif( x < min ) :  return min
    elif( x > max ) :  return max
    else :             return x


if __name__ == "__main__":
    if sys.argv[1] == "-1":
        read_points("out.txt")
    else:
        write_points(generatePolygon(7, 7, 4, .5, .5, int(sys.argv[1])), "out.txt")
