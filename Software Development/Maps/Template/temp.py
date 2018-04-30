import cv2
import numpy as np


def get_matches(img):
    r = 27
    for i in range(3, 6):
        cv2.imwrite("Crop"+str(i)+".jpg", img[75+r*i:99+r*i, 280:320])
    #cv2.waitKey(0)
    return img.shape


def get_matches1(img):
    r = 546//7-10
    for i in range(3):
        cv2.imwrite("Crops"+str(i)+".jpg", img[r*i:r*(1+i), ::])
    cv2.waitKey(0)
    return img.shape


def get_match(img, template):
    res = cv2.matchTemplate(img, template, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8
    loc = np.where(res >= threshold)
    for pt in zip(*loc[::-1]):
        cv2.rectangle(img, pt, (pt[0] + template.shape[1], pt[1] + template.shape[0]), 0, 2)
    return img


if __name__ == "__main__":
    # img = cv2.imread("SymbolSet.png", 0)
    # get_matches1(img)
    # get_matches(img1)
    for i in range(3):
        img = cv2.imread("Symbols.png", 0)
        template = cv2.imread("Crop"+str(i+3)+".jpg", 0)
        cv2.imwrite("Crop"+str(i)+".jpg", get_match(img, template))
