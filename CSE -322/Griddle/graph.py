import matplotlib.pyplot as plt
import numpy as np

def showPlot(fname):
    with open(fname) as f:
        lines = f.readlines()
        s=[]
        for i in lines:
            s.append(int(i))
    plt.plot(s)
    plt.ylabel('Muhit Voxod')
    plt.xlabel('Arnob Voxod')
    plt.show()

if __name__ == "__main__":
    showPlot("nasif.txt")
