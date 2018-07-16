import cv2
import numpy as np
import random as rn
from skimage.feature import canny
from skimage.transform import probabilistic_hough_line
from scipy.ndimage import grey_erosion, grey_dilation, generate_binary_structure


class Process:
    """
    This class contains all the algorithms used in getting different sub images
    """

    def __init__(self):
        pass

    idx = None

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
    def blurs(img, select=3):
        """
        Applies blurs to exterminate small noises in the image
        and then uses morphological closing to join lines
        :param select:
        :param img: image with noise
        :return: noise reduced image
        """
        if select == 0:
            blur = cv2.GaussianBlur(img, (3, 3), 0)
            # ret3, img = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
            img = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                        cv2.THRESH_BINARY_INV, 11, 2)
            img = cv2.GaussianBlur(img, (3, 3), 0)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel, iterations=2)
        elif select == 1:
            img = cv2.GaussianBlur(img, (5, 5), 0)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.dilate(img, kernel, iterations=1)
            img = cv2.erode(img, kernel, iterations=1)
        elif select == 2:
            kernel = np.ones((7, 7), np.uint8)
            img = cv2.erode(img, kernel, iterations=1)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.dilate(img, kernel, iterations=2)
        elif select == 3:
            blur = cv2.GaussianBlur(img, (3, 3), 0)
            ret3, img = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
            kernel = np.ones((3, 3), np.uint8)
            img = cv2.dilate(img, kernel, iterations=1)
        elif select == 4:
            img = cv2.GaussianBlur(img, (5, 5), 0)
            img = grey_erosion(img, size=(3, 3))
            img = grey_dilation(img, size=(3, 3))
        return img

    @staticmethod
    def get_edges(img):
        """
        A better edge detection scheme
        :param img: Image where edges are not coherent
        :return: image where edges are highlighted
        """
        reduced_noise = Process.blurs(img, 1)
        img = cv2.Canny(reduced_noise, 100, 200)
        return ~img

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
        img, contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        if flag == 1:
            empty = np.zeros(img.shape, np.uint8)
            for cnt in contours[1:]:
                cv2.drawContours(empty, [cnt], 0, (255, 255, 255), -1)
                i, j, k = rn.randint(0, 255), rn.randint(0, 255), rn.randint(0, 255)
                cv2.drawContours(color, [cnt], 0, (i, j, k), -1)
                empty = np.zeros(img.shape, np.uint8)
            return color, contours[1:]
        else:
            return img, contours[1:]

    @staticmethod
    def get_subplots(img_org, img_plot):
        """
        This gives the subplots of the map
        :param img_org: original image
        :param img_plot: image without the plot lines
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
    def get_split(img, contour_size=1200):
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

        plots = np.zeros_like(img)
        plots[plots == 0] = 255
        nums = np.zeros_like(img)
        nums[nums == 0] = 255

        for cnt in contours[2:]:
            now_size = cv2.contourArea(cnt)
            max_contour_size = max(now_size, max_contour_size)
            min_contour_ize = min(now_size, min_contour_ize)
            if now_size > contour_size:
                cv2.drawContours(plots, [cnt], 0, 0, 2)
            elif now_size > contour_size/10:
                cv2.drawContours(nums, [cnt], 0, 0, -1)

        plots = Process.blurs(plots, 2)
        return plots, nums

    @staticmethod
    def find_hough(image):
        """
        Applied probabilistic Hough Transform to obtain straight lines
        :param image:
        :return:
        """
        image = Process.blurs(image, 1)
        edges = canny(image, 2, 1, 25)
        lines = probabilistic_hough_line(edges, threshold=10, line_length=1,
                                         line_gap=10)
        # print(len(lines))
        img = np.zeros_like(image)
        img[img == 0] = 255
        for line in lines:
            p0, p1 = line
            cv2.line(img, (p0[0], p0[1]), (p1[0], p1[1]), (0, 0, 0), 5)
        return img

    @staticmethod
    def get_original(loc):
        """
        The pixel is found on the original image and extracted from it
        :param loc: pixel location
        :return: extracted image
        """

    @staticmethod
    def create_idx(img):
        """
        Stores a mapping for img pixel neighbors to pixel
        :param img: original image
        :return: NIL
        """
        dim = max(img.shape[0], img.shape[1])
        a = np.repeat(np.arange(dim), dim).reshape((dim, dim))
        Process.idx = np.dstack((a, a.T))

    @staticmethod
    def getn(x, y, coord, d=1):
        """
        returns the neighboring indices or the values of coord
        :param x: shape[0]
        :param y: shape[1]
        :param coord: pixel location
        :param d: window size
        :return: array of indices
        """
        i, j = coord
        return Process.idx[max(i - d, 0):min(i + d + 1, x), max(j - d, 0):min(j + d + 1, y)].flatten()

    @staticmethod
    def region_growing(seed):
        """
        Samples pixel from the original image from the provided seed point list
        and shows its connected region
        :param seed:
        :return: image containing only seed-connected region
        """

    @staticmethod
    def seed_selection(cnt):
        """
        This method returns a number of seeds from the separated contours that are smaller
        in area the selection probability is proportional to the inverse of its area
        :param cnt: List of contours with small size
        :return: list of seed points
        """

    @staticmethod
    def endings(img):
        """
        takes a morphological skeleton of an image and finds probable list of end-points: points
        where edges terminate abruptly
        :param img: skeleton image
        :return: probable line-endings
        """

    @staticmethod
    def store_tree(tree):
        """
        Stores the detected contours in a tree structure so that the neighboring information
        can be restored from non-volatile memory such as
        :param tree: contour tree
        :return: NIL
        """

    @staticmethod
    def fitness_function_1(man, auto):
        """
        Man and Auto are Manually and automatically generated images respectively. This fitness
        function calculates the unlinked broken edges
        :param man: Manually linked image
        :param auto: Automatically linked image
        :return: error rate
        """

    @staticmethod
    def fitness_function_2(man, auto):
        """
        Man and Auto are Manually and automatically generated images respectively. This fitness
        function calculates the number of unrecognized regions
        :param man: Manually segmented image
        :param auto: Automatically segmented image
        :return: error rate
        """

    @staticmethod
    def fidelity():
        """
        Uses the fitness functions to determine the next course of action
        :return:
        """

    @staticmethod
    def symbolize(nums):
        """
        Classifies Text/Digit/Symbols based on morphology using SIFT
        :param nums: Separated nums field
        :return: dictionary mapping object to string
        """

    @staticmethod
    def synthetic_data(cnts):
        """
        List of contours are provided; from which synthetic data for digit and Symbol recognition
        is generated
        :param cnts: List of contours
        :return: dictionary
        """

