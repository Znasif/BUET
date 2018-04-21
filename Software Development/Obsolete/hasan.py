import numpy as np
import scipy.misc as ms
import cv2
import random

if __name__ == "__main__":
    file = input("File Location : ")
    n=int(input("Iterations : "))
    img = cv2.imread(file)
    file1,file2=file.split('.')
    a,b,_=img.shape
    c=int(((a/2)**2+(b/2)**2)**.5)
    d,e=c-a//2,c-b//2
    temp=np.zeros((2*c,2*c,3))
    temp[d:d+a,e:e+b]=img
    for i in range(n):
        ang = random.randint(0,360)
        img = ms.imrotate(temp,ang)
        cv2.imwrite(file1+'_'+str(i+1)+'.'+file2,img)