import cv2
import numpy as np

a=cv2.imread('Maps/plot.bmp',0)
b=cv2.imread('Maps/nums.bmp',0)

p,q=np.shape(a)

new=np.full((p,q,3),(255,255,255))

new[a==0]=(255,255,0)
new[b==0]=(255,0,255)

cv2.imwrite('Maps/see.bmp',new)
