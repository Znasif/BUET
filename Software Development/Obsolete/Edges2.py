import numpy as np
import matplotlib.pyplot as plt
import cv2
import pycurl

c = pycurl.Curl()
c.setopt(c.URL, 'http://pycurl.io/tests/testfileupload.php')

c.setopt(c.HTTPPOST, [
    ('fileupload', (
        c.FORM_BUFFER, 'readme.txt',
        c.FORM_BUFFERPTR, 'This is a fancy readme file',
    )),
])

c.perform()
c.close()
# img = cv2.imread('a1.jpg')
# edge = cv2.Canny(img, 100, 200)
# #plt.subplot(122)
# plt.imshow(edge,cmap='gray')
# plt.show()

