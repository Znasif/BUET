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
        plt.imshow(image, cmap=a_map)
        plt.title(title)
        plt.axis('off')
        Visual.image = image
        plt.show()

    @staticmethod
    def show(title, image):
        """
        Show image in new window

        :param title: Title of image
        :param image: the numpy array to represent
        :return: NIL
        """
        Visual.image = image
        cv2.imshow(title, Visual.image)

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

        new = np.full((p, q, 3), (255, 255, 255))

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
        if event == cv2.EVENT_LBUTTONDBLCLK:
            cv2.circle(Visual.image, (x, y), 1, 0, -1)
            Visual.clicks.append((y, x))

    @staticmethod
    def get_pixel(img):
        cv2.namedWindow('Connect Broken Edges')
        cv2.setMouseCallback('Connect Broken Edges', Visual.on_mouse, 0)
        while True:
            Visual.show('Connect Broken Edges', img)
            pressed_key = cv2.waitKey(20) & 0xFF
            if pressed_key == ord('q'):
                cv2.destroyAllWindows()
