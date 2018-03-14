import numpy as np
import cv2

img = cv2.imread('1.jpg')
height, width = img.shape[:2]
#gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
#gray = np.float32(gray)

#n = np.zeros((height,width, 3), np.uint8)
#n[:] = (255,255,255)

edges = cv2.Canny(img,100,200)

cv2.imshow('Edges',edges)
cv2.imwrite('edges.jpg', edges)
cv2.waitKey(0)
