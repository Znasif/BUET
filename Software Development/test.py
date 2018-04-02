import numpy as np
import matplotlib.pyplot as plt
import cv2

def plts(title, image, map='gray'):
    plt.imshow(image, cmap=map)
    plt.title(title)
    plt.show()

img = cv2.imread('original.jpg',0)
new = np.ones_like(img)
new[new==1]=255
new[img<180]=0
spare=new
new=cv2.GaussianBlur(new,(5,5),0)
kernel = np.ones((3,3),np.uint8)
new = cv2.dilate(new,kernel,iterations=1)
new = cv2.erode(new,kernel,iterations=1)
ret, img = cv2.threshold(new, 180, 255, 0)
im2, contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
min1=110000
max1=-1
empty=np.empty_like(img)
empty[empty==0]=255
nums=np.empty_like(img)
nums[nums==0]=255
for cnt in contours[2:]:
    a=cv2.contourArea(cnt)
    max1=max(a,max1)
    min1=min(a,min1)
    if(a>1000):
        cv2.drawContours(empty,[cnt], 0, 0, 2)
    else:
        cv2.drawContours(nums,[cnt], 0, 0, -1)
kernel = np.ones((7,7),np.uint8)
empty = cv2.erode(empty,kernel,iterations=1)
kernel = np.ones((3,3),np.uint8)
empty = cv2.dilate(empty,kernel,iterations=5)
cv2.imwrite('plot.jpg',empty)
#empty = ~(empty ^ spare)
#empty = cv2.dilate(empty,kernel,iterations=1)
#empty = cv2.erode(empty,kernel,iterations=2)

#print(max1,min1)
#cv2.imshow('New',empty)
cv2.imwrite('nums.jpg',nums)
cv2.waitKey()
