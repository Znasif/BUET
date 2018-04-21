import numpy as np
import cv2

img = cv2.imread('a1.jpg')
height, width = img.shape[:2]
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
gray = np.float32(gray)

corners = cv2.goodFeaturesToTrack(gray, 1000, 0.01, 10)
corners = np.int0(corners)
f = open("corners.txt","w+")
n = np.zeros((height,width, 3), np.uint8)
n[:] = (255,255,255)
i=0
for corner in corners:
    x,y = corner.ravel()
    i+=1
    f.write(str(i)+" x = :"+str(x)+"  y = :"+str(y)+"\n")
    cv2.circle(img,(x,y),3,255,-1)
    cv2.circle(n,(x,y),3,255,-1)
f.close()
cv2.imshow('Corner',img)
cv2.imwrite('corners.jpg', n)
cv2.waitKey(0)
