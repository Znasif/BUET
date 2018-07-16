import numpy as np
import matplotlib.pyplot as plt
import cv2


class Visual:
    """
    This class mainly deals with visualising the different
    processes that take place
    """
    def __init__(self):
        pass

    in_folder = "Maps/"
    out_folder = "Extracted/"
    image = None
    current = [0, 0]
    shift = False
    clicks = []

    @staticmethod
    def image_open(title, flag=0):
        """
        Open image file

        :param title: Open image with title as name
        :param flag: 0: default to binary 1: color image
        :return: numpy array of image
        """
        img = cv2.imread(Visual.in_folder+title, flag)
        # cv2.imshow(Visual.in_folder+title, img)
        # cv2.waitKey()
        Visual.image = img
        return img

    @staticmethod
    def image_write(title, img):
        """
        Write image to file

        :param title: name of image file to write to
        :param img: numpy array to write
        :return: NIL
        """
        Visual.image = img
        cv2.imwrite(Visual.out_folder+title.split(".")[0]+".tif", img)

    @staticmethod
    def plot(title, image, a_map='gray'):
        """
        Show image in new window

        :param title: Title of image
        :param image: the numpy array to represent
        :param a_map: default to greyscale image
        :return: NIL
        """
        plt.title(title)
        plt.axis('off')
        if len(image.shape) == 3:
            plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
        else:
            plt.imshow(image, cmap=a_map)
        Visual.image = image
        plt.show()

    @staticmethod
    def show(title, image):
        """
        Show image in new window with shifting window

        :param title: Title of image
        :param image: the numpy array to represent
        :return: NIL
        """
        Visual.image = image
        a, b = 600, 1200
        if Visual.shift:
            Visual.current[1] += b
            if Visual.current[1] > image.shape[1]:
                Visual.current[1] = 0
                Visual.current[0] += a
                if Visual.current[0] > image.shape[0]:
                    Visual.current[0] = 0
        mx, my = min(image.shape[0], Visual.current[0]+a), min(image.shape[1], Visual.current[1]+b)
        show_image = image[Visual.current[0]:mx, Visual.current[1]:my]
        cv2.imshow(title, show_image)

    @staticmethod
    def get_nums(im_org, im_plot):
        """
        Subtract plot contour image from original to get
        image with only numbers

        :param im_org: original image
        :param im_plot: plot contour image
        :return: numbered image
        """
        im_nums = ~(im_org ^ im_plot)
        # Visual.plot('Contour', im_plot)
        # Visual.plot('Numbers and Contour', im_org)
        Visual.plot('Numbers', im_nums)
        Visual.image = im_nums
        return im_nums

    @staticmethod
    def get_overlay(im_org, im_plot):
        """
        Shows overlap of the two images in different colors
        :param im_org: image 1
        :param im_plot: image 2
        :return: NIL
        """
        p, q = np.shape(im_org)

        new = np.full((p, q, 3), (255, 255, 255), dtype=np.uint8)

        new[im_org == 0] = (255, 255, 0)
        new[im_plot == 0] = (255, 0, 255)
        Visual.image = new
        return new

    @staticmethod
    def draw_contour(img_org, contour):
        """
        Just an interface for drawContour function
        :param img_org: original image
        :param contour: contour to be drawn
        :return: image with just the contour
        """

        empty = np.zeros(img_org.shape, np.uint8)
        empty = ~empty
        cv2.drawContours(empty, [contour], 0, 0, 1)
        Visual.image = empty
        return empty

    @staticmethod
    def on_mouse(event, x, y, flags, params):
        """
        Select seed Points using Left Mouse Click
        :param event: Left Mouse Click
        :param x: Column
        :param y: Row
        :param flags: NIL
        :param params: NIL
        :return: stores (y, x) into clicks
        """
        if event == cv2.EVENT_LBUTTONDOWN:
            cv2.circle(Visual.image, (Visual.current[1] + x, Visual.current[0] + y), 1, 0, -1)
            Visual.clicks.append((Visual.current[0] + y, Visual.current[1] + x))

    @staticmethod
    def get_pixel(img, title='Collect Seed'):
        """
        Selection of seed point for 1. Edge linking 2. Region growing etc.
        :param img: Original image
        :param title: Window Name
        :return: NIL
        """
        cv2.namedWindow(title)
        cv2.setMouseCallback(title, Visual.on_mouse, 0)
        Visual.current = [0, 0]
        Visual.shift = False
        while True:
            Visual.show(title, img)
            Visual.shift = False
            pressed_key = cv2.waitKey(20) & 0xFF
            # print(chr(pressed_key))
            if pressed_key == ord('q'):
                cv2.destroyAllWindows()
                break
            elif pressed_key == ord('a'):
                Visual.shift = True
            elif pressed_key == ord('c'):
                Visual.connect_line()

    @staticmethod
    def connect_line():
        """
        Connect points from Clicks with 1. Lines 2. Extrapolated Curves
        :return: NIL
        """
        ln = len(Visual.clicks)
        for i in range(0, ln, 2):
            y, x = Visual.clicks[i]
            y_, x_ = Visual.clicks[i]
            if i + 1 < ln:
                y_, x_ = Visual.clicks[i+1]
            Visual.image = cv2.line(Visual.image, (x, y), (x_, y_), 0, 2)

    @staticmethod
    def plot_fidelity(x, y, seed_cnt):
        """
        Plot fitness functions vs seed_cnt
        :param x: fitness_function_1
        :param y: fitness_function_2
        :param seed_cnt: number of provided seeds
        :return: NIL
        """
