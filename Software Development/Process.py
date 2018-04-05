import cv2
import numpy as np
import random as rn


class Process:
    """
    This class contains all the algorithms used in getting different sub images
    """

    def __init__(self):
        pass

    @staticmethod
    def blurs(img):
        """
        Applies blurs to exterminate small noises in the image
        :param img: image with noise
        :return: noise reduced image
        """
        blur = cv2.GaussianBlur(img, (3, 3), 0)
        ret3, img = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
        img = cv2.medianBlur(img, 3)
        return img

    @staticmethod
    def thinning(img):
        """
        Thin the black lines
        :param img: original image
        :return: eroded image
        """
        kernel = np.ones((5, 5), np.uint8)
        img = cv2.erode(img, kernel, iterations=1)
        return img

    @staticmethod
    def get_contour(img, color, flag=0):
        """
        This derives the available contours of an image
        :param self: NIL
        :param image: the image with contours
        :param flag: 1: Show image 0: just return contours
        :return:
        """
        # apply blur
        img = Process.blurs(img)

        # apply thinning
        img = Process.thinning(img)

        # get contours
        ret, thresh = cv2.threshold(img, 127, 255, 0)
        im2, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        if flag:
            empty = np.zeros(img.shape, np.uint8)
            for cnt in contours[1:]:
                cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
                cv2.drawContours(color, [cnt], 0, (rn.randint(0, 255), rn.randint(0, 255), rn.randint(0, 255)), -1)
                empty = np.zeros(img.shape, np.uint8)
            return color, contours[1:]
        else:
            return im2, contours[1:]

