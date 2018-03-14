import numpy as np
import cv2
import selectRef

img = cv2.imread('1.jpg')
img = cv2.resize(img, (0,0), fx=0.5, fy=0.5)

# height, width = img.shape[:2]
# gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
# gray = np.float32(gray)

f = open("corners.txt", "w+")
flagy = 0
i=0

def draw_circle(event, x, y, flags, param):
    global mouseX, mouseY
    global axx,cxx,origin
    global i,f,flagy

    if event == cv2.EVENT_LBUTTONDBLCLK:
        cv2.circle(img, (x, y), 3, 255, -1)
        mouseX, mouseY = x, y
        if(flagy==0):
            #a,b = getFirst().split()
            a,b=23.59718,89.84191
            axx = selectRef.point(float(a),float(b),mouseX, mouseY)
            flagy=1
            print("HERE")
        elif(flagy==1):
            #a,b = getSecond().split()
            a,b=23.59728,89.84198
            cxx = selectRef.point(float(a),float(b),mouseX, mouseY)
            flagy=2
            origin = selectRef.calculate_origin(axx,cxx)
            origin.print_latlon()
            print("HERE 1")
        else:
            #i = getCurrentLandID()
            #cv2.circle(img, (x, y), 100, (255, 0, 0), -1)
            new_point = selectRef.calculate(origin,mouseX, mouseY)
            print("HERE 2")
            f.write(str(i) + " " + str(new_point.lat) + " " + str(new_point.lon) + "\n")

cv2.namedWindow("Points")
#cv2.namedWindow("Points", cv2.WND_PROP_FULLSCREEN)
cv2.setMouseCallback("Points", draw_circle)
#cv2.setWindowProperty("Points",cv2.WND_PROP_FULLSCREEN,cv2.WINDOW_FULLSCREEN)
while (1):
    cv2.imshow('Points', img)
    k = cv2.waitKey(20) & 0xFF
    if(k==ord('q')):
        break
f.close()
