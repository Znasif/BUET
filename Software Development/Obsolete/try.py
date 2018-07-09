import cv2
import scipy.ndimage as ni
import numpy as np
from skimage import filters

# img = cv2.imread('SymbolSet.png', 0)
img = cv2.imread('port.jpg', 0)
'''img = np.array([[10, 50, 10, 50, 10],
                [10, 55, 10, 55, 10],
                [10, 65, 10, 65, 10],
                [10, 50, 10, 50, 10],
                [10, 55, 10, 55, 10]])
'''
k = np.array([[-1, -2, -1],
              [0, 0, 0],
              [1, 2, 1]])
# img1 = ni.correlate(img, k, mode='mirror')
img1 = np.array(filters.sobel(img))
cv2.imwrite('test.jpg', img+img1)
# img = cv2.imread('test.png', 0)
# img = np.invert(img)
cv2.imshow('test', img)
cv2.waitKey(0)