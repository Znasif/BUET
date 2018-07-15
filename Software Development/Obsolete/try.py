import numpy as np
import scipy.ndimage as ndi
import cv2
'''
im = np.array([[10, 50, 10, 50, 10],
               [10, 55, 10, 55, 10],
               [10, 65, 10, 65, 10],
               [10, 50, 10, 50, 10]])

k = np.array([[1, 1, 1],
              [1, 0, 1],
              [1, 1, 1]])


def create_idx(img):
    dim = max(img.shape[0], img.shape[1])
    a = np.repeat(np.arange(dim), dim).reshape((dim, dim))
    return np.dstack((a, a.T))


def getn(img, idx, coord, d=1):
    i, j = coord
    return idx[max(i - d, 0):min(i + d + 1, img.shape[0]),
           max(j - d, 0):min(j + d + 1, img.shape[1])]


#i, j = input().split()
#i, j = int(i), int(j)
p = create_idx(im)
#rs = getn(im, p, (i, j))
a = np.array((0, 1))
print(p, a)
# rs = ndi.generic_filter(img, )
'''
a = np.zeros(600, 1200)
cv2.imshow('name', a)

cv2.waitKey()