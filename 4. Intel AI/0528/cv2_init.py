import numpy as np
import cv2

# image read
img = cv2.imread("my_input.jpeg")

# image display window
cv2.namedWindow("image", cv2.WINDOW_NORMAL)

# np array 
print(img.shape)

# image display
cv2.imshow("image", img)

# wait for input key
cv2.waitKey(0)

# output.png > image save
cv2.imwrite("output.png", img)

# destroy all window
cv2.destroyAllWindows()


#######################################################


# image read, color space print
color = cv2.imread("strawberry.jpg", cv2.IMREAD_COLOR)

print(color.shape)

height, width, channels = color.shape
cv2.imshow("Original Image", color)


# color channel > R, G, B
b, r, g = cv2.split(color)
rgb_split = np.concatenate((b, r, g), axis=1)
cv2.imshow("BGR Channels ", rgb_split)

# RGB > HSV
hsv = cv2.cvtColor(color, cv2.COLOR_BGR2HSV)

# Channel > H, S, V
h, s, v = cv2.split(hsv)
hsv_split = np.concatenate((h, s, v), axis=1)
cv2.imshow("Split HSV", hsv_split)
cv2.waitKey(0)
cv2.destroyAllWindows()

########################################################

# image read
img = cv2.imread("my_input.jpeg")

# Crop
cropped = img[50:450, 100:400]

# resize
resized = cv2.resize(cropped, (400, 200))

# display
cv2.imshow("Original", img)
cv2.imshow("Cropped image", cropped)
cv2.imshow("Resized image", resized)

cv2.waitKey(0)
cv2.destroyAllWindows()

#########################################################

dst = cv2.bitwise_not(img)

cv2.imshow("src", img)
cv2.imshow("dst", dst)
cv2.waitKey(0)
cv2.destroyAllWindows()

#########################################################

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ret, dst = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)

cv2.imshow("dst", dst)
cv2.waitKey()
cv2.destroyAllWindows()

#########################################################

dst = cv2.blur(img, (9, 9), anchor=(-1, -1), borderType=cv2.BORDER_DEFAULT)

cv2.imshow("dst", dst)
cv2.waitKey()
cv2.destroyAllWindows()

#########################################################

sobel = cv2.Sobel(gray, cv2.CV_8U, 1, 0, 3)

cv2.imshow("sobel", sobel)
cv2.waitKey()
cv2.destroyAllWindows()

##########################################################

src = cv2.imread("rainbow.jpg", cv2.IMREAD_COLOR)
hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
h, s, v = cv2.split(hsv)

lower_red = cv2.inRange(hsv, (0, 100, 100), (5, 255, 255))
upper_red = cv2.inRange(hsv, (170, 100, 100), (180, 255, 255))
added_red = cv2.addWeighted(lower_red, 1.0, upper_red, 1.0, 0.0)

red = cv2.bitwise_and(hsv, hsv, mask = added_red)
red = cv2.cvtColor(red, cv2.COLOR_HSV2BGR)

cv2.imshow("red", red)
cv2.waitKey(0)
cv2.destroyAllWindows()

##########################################################

b, g, r = cv2.split(src)
inverse = cv2.merge((r, g, b))

cv2.imshow("b", b)
cv2.imshow("g", g)
cv2.imshow("r", r)
cv2.imshow("inverse", inverse)
cv2.waitKey(0)
cv2.destroyAllWindows()

