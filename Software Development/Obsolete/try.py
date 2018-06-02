import cv2
import matplotlib.pyplot as plt
import numpy as np
from skimage import measure, morphology, filters

trial = cv2.imread('trial.jpg')
plt.imshow(trial)
plt.show()

trial_erode = morphology.binary_erosion(trial, selem=morphology.disk(7))
plt.imshow(trial_erode)
plt.show()











