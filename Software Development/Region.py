import cv2
import numpy as np
from matplotlib import pyplot as plt


def plts(title, image, map='gray'):
    plt.imshow(image, cmap=map)
    plt.title(title)
    plt.show()


def get8n(x, y, shape):
    out = []
    maxx = shape[1] - 1
    maxy = shape[0] - 1

    # top left
    outx = min(max(x - 1, 0), maxx)
    outy = min(max(y - 1, 0), maxy)
    out.append((outx, outy))

    # top center
    outx = x
    outy = min(max(y - 1, 0), maxy)
    out.append((outx, outy))

    # top right
    outx = min(max(x + 1, 0), maxx)
    outy = min(max(y - 1, 0), maxy)
    out.append((outx, outy))

    # left
    outx = min(max(x - 1, 0), maxx)
    outy = y
    out.append((outx, outy))

    # right
    outx = min(max(x + 1, 0), maxx)
    outy = y
    out.append((outx, outy))

    # bottom left
    outx = min(max(x - 1, 0), maxx)
    outy = min(max(y + 1, 0), maxy)
    out.append((outx, outy))

    # bottom center
    outx = x
    outy = min(max(y + 1, 0), maxy)
    out.append((outx, outy))

    # bottom right
    outx = min(max(x + 1, 0), maxx)
    outy = min(max(y + 1, 0), maxy)
    out.append((outx, outy))

    return out


def region_growing(img, seed):
    list = seed
    outimg = np.ones_like(img)
    outimg[outimg==1]=255
    processed = []
    img_thresh = int(input("Enter Threshold Value: "))
    while (len(list) > 0):
        pix = list[0]
        outimg[pix] = 255
        for coord in get8n(pix[0], pix[1], img.shape):
            if coord not in processed and img[coord] > img_thresh:
                #print(img[coord],end=" ")
                outimg[coord] = 255
                list.append(coord)
                processed.append(coord)
        #print()
        list.pop(0)
        # cv2.imshow("progress",outimg)
        # cv2.waitKey(1)
    print("Done")
    return outimg


def on_mouse(event, x, y, flags, params):
    global clicks,img
    if event == cv2.EVENT_LBUTTONDBLCLK:
        cv2.circle(img, (x, y), 1, 0, -1)
        print('Seed: ' + str(x) + ', ' + str(y), img[y, x])
        clicks.append((y, x))


def ex1():
    global clicks,img
    #ret, img = cv2.threshold(img, 128, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    #hist,bins = np.histogram(img.ravel(),256,[0,256])
    #print(hist)
    #plt.hist(img.ravel(),256,[0,256])
    #plt.show()
    img=cv2.GaussianBlur(img,(15,15),0)
    img=cv2.medianBlur(img,15)
    # plts('Seed Points', img)
    cv2.namedWindow('Seed Points')
    cv2.setMouseCallback('Seed Points', on_mouse, 0)
    while (1):
        cv2.imshow('Seed Points', img)
        k = cv2.waitKey(20) & 0xFF
        if (k == ord('q')):
            print("STOP")
            out = region_growing(img, clicks)
            cv2.imshow('Region Growing', out)
            # cv2.imwrite('Extracted/p.bmp',out)
            # cv2.destroyAllWindows()
            break
    cv2.waitKey()
    cv2.destroyAllWindows()

def get_regions(img):
    #connected_components=[]
    outimg = np.ones_like(img)
    outimg[outimg==1]=255
    processed = np.empty_like(img)
    img_thresh = 180  #int(input("Enter Threshold Value: "))
    a,b=np.shape(img)
    for i in range(a):
        for j in range(b):
            if processed[i][j]==0 and img[i][j]>img_thresh:
                for coord in get8n(i,j,img.shape):
                    if processed[coord]==0:
                        outimg[coord]=0
                        processed[coord]=1
    return outimg

def ex2():
    global img
    kernel = np.ones((15,15),np.uint8)
    img = cv2.erode(img,kernel,iterations = 1)
    img = cv2.dilate(img,kernel,iterations = 1)
    cv2.imshow('1', img)
    cv2.waitKey()

def ex3():
    img = cv2.GaussianBlur(img, (15, 15), 0)
    img = cv2.medianBlur(img, 15)
    cv2.imshow('1', img)
    cv2.waitKey()
    new = get_regions(img)
    cv2.imwrite('Maps/region.jpg', new)
    # cv2.waitKey()

if __name__ == "__main__":
    clicks = []
    img = cv2.imread('Maps/port.jpg', 0)
    ex1()