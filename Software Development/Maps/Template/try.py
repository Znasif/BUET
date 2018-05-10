import cv2
import numpy as np


def get_matches(img):
    x, y = img.shape
    r = x//11
    r1 = r//8

    l = [(2*r1, 2*r1), (r1, r1), (0, 0), (0, 0), (r1, r1), (r1, r1), (0, 0), (0, 0), (0, 0), (-r1, -r1), (-r1, -r1)]

    for i in range(11):
        a, b = l[i]
        cv2.imwrite("Crop"+str(i)+".jpg", img[r*i+a:r + r*i+b, :7*y//8])
    #cv2.waitKey(0)
    return img.shape


def get_matches1(img):
    r = 546//7-10
    for i in range(3):
        cv2.imwrite("Crops"+str(i)+".jpg", img[r*i:r*(1+i), ::])
    cv2.waitKey(0)
    return img.shape


def get_match(img1, template):
    img = np.copy(img1)
    res = cv2.matchTemplate(img, template, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8
    loc = np.where(res >= threshold)
    for pt in zip(*loc[::-1]):
        cv2.rectangle(img, pt, (pt[0] + template.shape[1], pt[1] + template.shape[0]), 0, 2)
    return img

def get_match_with_SIFT(img1, template):
    k, d = cv2.xfeatures2d.SIFT_create()

if __name__ == "__main__":
    img = cv2.imread("big.jpg", 0)
    a, b = img.shape
    a, b = a // 8, b // 8
    r = 1000
    j = 200
    img1 = img[r:a + r-j, -b:]
    a, b = img1.shape
    img2 = img1[:, b//4:b//2]
    cv2.imwrite('find.jpg', img2)
    img3 = get_matches(img2)
    """
    for i in range(11):
        template = cv2.imread("Crop" + str(i) + ".jpg", 0)
        cv2.imwrite("Crops" + str(i) + ".jpg", get_match(img, template))
    """

