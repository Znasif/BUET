import numpy as np
import scipy.ndimage as ndi

img = np.array([[10, 50, 10, 50, 10],
                [10, 55, 10, 55, 10],
                [10, 65, 10, 65, 10],
                [10, 50, 10, 50, 10],
                [10, 55, 10, 55, 10]])

k = np.array([[1, 1, 1],
              [1, 0, 1],
              [1, 1, 1]])

i, j, d = input().split()
i, j, d = int(i), int(j), int(d)
rs = img[i-d:i+d+1, j-d:j+d+1].flatten()
print(rs)
rs = ndi.generic_filter(img, )