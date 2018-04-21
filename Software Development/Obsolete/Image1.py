import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('1.jpg',0)
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()

# plt.imshow(img, cmap='gray', interpolation='bicubic')
# plt.plot([50,100],[80,100],'c',linewidth=5)
# plt.show()