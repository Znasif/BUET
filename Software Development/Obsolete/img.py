import cv2
import numpy as np
import matplotlib.pyplot as plt
import random as rn


def plts(title, image, map='gray'):
    plt.imshow(image, cmap=map)
    plt.title(title)
    plt.show()


def getContour(image, flag):
    img = cv2.imread(image, 0)
    color = cv2.imread(image)
    blur = cv2.GaussianBlur(img, (3, 3), 0)
    ret3, img = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    img = cv2.medianBlur(img, 3)

    kernel = np.ones((5, 5), np.uint8)

    img = cv2.erode(img, kernel, iterations=1)
    # mask = np.zeros(img.shape, np.uint8)
    empty = np.zeros(img.shape, np.uint8)
    ret, thresh = cv2.threshold(img, 127, 255, 0)
    im2, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    if (flag):
        for cnt in contours[1:]:
            cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
            cv2.drawContours(color, [cnt], 0, (rn.randint(0, 255), rn.randint(0, 255), rn.randint(0, 255)), -1)
            #plts('Steps',empty)
            empty = np.zeros(img.shape, np.uint8)
        return color, contours[1:]
    else:
        return im2, contours[1:]


def maskit(img, num, con):
    can = np.zeros(img.shape)
    for i in con:
        empty = np.zeros(con.shape[:2])
        cv2.drawContours(empty, [i], 0, 255, -1)
        cv2.imshow("H", empty)


def ex1():
    pic = ['Maps/Numbered.png', 'Maps/Enhanced.png', 'Extracted/Numbered.png', 'Extracted/Enhanced.png']
    color, con = getContour(pic[1], 1)
    for cnt in con:
        img = cv2.imread(pic[0], 0)
        #print(cv2.contourArea(cnt))
        empty = np.zeros(img.shape, np.uint8)
        cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
        empty=~empty
        img=img|empty
        plts("New", img)
        im2, contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        print("New")
        for p in contours[1:]:
            q=cv2.contourArea(p)
            if(q>4000):
                print(q)
                empty = np.zeros(img.shape[:2])
                cv2.drawContours(empty, [p], 0, 255, -1)
                #plts("Inside",empty)
        #plts("Steps Numbers",img)

    plts("Shaded",color)


def ex2():
    pic = ['Maps\\Numbered.png', 'Maps\Enhanced.png', 'Extracted\\Numbered.png', 'Extracted\Enhanced.png']
    img = cv2.imread(pic[1], 0)
    img2 = cv2.imread(pic[0], 0)
    empty = ~(img ^ img2)
    plts('Contour', img)
    plts('Numbers and Contour', img2)
    plts('Numbers', empty)


def ex3():
    pic = ['Maps\\Numbered.png', 'Maps\Enhanced.png', 'Extracted\\Numbered.png', 'Extracted\Enhanced.png']
    img = cv2.imread(pic[1], 0)
    img2 = cv2.imread(pic[0], 0)
    color, con = getContour(pic[1], 1)

def ex4():
    pic = ['Maps\\Numbered.png', 'Maps\Enhanced.png', 'Extracted\\Numbered.png', 'Extracted\Enhanced.png']


if __name__ == "__main__":
    ex1()