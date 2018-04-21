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
    def get_bin(img, threshold):
        """
        Get binary image from Greyscale image
        :param threshold: all greyscale value less than threshold gets zeroed
        :param img: Greyscale image
        :return: binary image
        """
        new = np.ones_like(img)
        new[new == 1] = 255
        new[img < threshold] = 0
        return new

    @staticmethod
    def blurs(img, select=0):
        """
        Applies blurs to exterminate small noises in the image
        and then uses morphological closing to join lines
        :param select:
        :param img: image with noise
        :return: noise reduced image
        """
        if select == 0:
            blur = cv2.GaussianBlur(img, (3, 3), 0)
            ret3, img = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
            img = cv2.medianBlur(img, 3)
            kernel = np.ones((5, 5), np.uint8)
            img = cv2.erode(img, kernel, iterations=1)
        elif select == 1:
            img = cv2.GaussianBlur(img, (5, 5), 0)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.dilate(img, kernel, iterations=1)
            img = cv2.erode(img, kernel, iterations=1)
        elif select == 2:
            kernel = np.ones((7, 7), np.uint8)
            img = cv2.erode(img, kernel, iterations=1)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.dilate(img, kernel, iterations=5)
        return img

    @staticmethod
    def get_contour(img, flag=0, color=0):
        """
        This derives the available contours of an image
        :param color: contains the colored contours : not needed when flag=0
        :param img: the image with contours
        :param flag: 1: Show image 0: just return contours
        :return:
        """
        # apply blur and thinning
        img = Process.blurs(img)

        # get contours
        ret, thresh = cv2.threshold(img, 127, 255, 0)
        img, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        if flag == 1:
            empty = np.zeros(img.shape, np.uint8)
            for cnt in contours[1:]:
                cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
                cv2.drawContours(color, [cnt], 0, (rn.randint(0, 255), rn.randint(0, 255), rn.randint(0, 255)), -1)
                empty = np.zeros(img.shape, np.uint8)
            return color, contours[1:]
        else:
            return img, contours[1:]

    @staticmethod
    def get_subplots(img_org, img_plot):
        """
        This gives the subplots of the map
        :param img_org: original image
        :param img_plot: image with ust the plot lines
        :return: list of numpy arrays of sub-images
        """
        _, contours = Process.get_contour(img_plot)
        subplot_list = []
        for cnt in contours:
            img = np.copy(img_org)
            # print(cv2.contourArea(cnt))
            empty = np.zeros(img.shape, np.uint8)
            cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
            empty = ~empty
            img = img | empty
            subplot_list.append(np.copy(img))
        return subplot_list, contours

    @staticmethod
    def get_split(img, contour_size=1000):
        """
        This is the first step to get two images: One with plot and another with nums
        based on the contour sizes

        :param img: original image
        :param contour_size: threshold contour size
        :return: two numpy arrays
        """
        # apply blur and thinning
        img = Process.blurs(img, 1)

        # get contours
        ret, img = cv2.threshold(img, 180, 255, 0)
        im2, contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        max_contour_size = 110000
        min_contour_ize = -1

        plots = np.empty_like(img)
        plots[plots == 0] = 255
        nums = np.empty_like(img)
        nums[nums == 0] = 255

        for cnt in contours[2:]:
            now_size = cv2.contourArea(cnt)
            max_contour_size = max(now_size, max_contour_size)
            min_contour_ize = min(now_size, min_contour_ize)
            if now_size > contour_size:
                cv2.drawContours(plots, [cnt], 0, 0, 2)
            else:
                cv2.drawContours(nums, [cnt], 0, 0, -1)

        plots = Process.blurs(plots, 2)
        return plots, nums
