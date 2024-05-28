import cv2

# Read from the first camera device
cap = cv2.VideoCapture(0)

topLeft = (50, 300)
bold = 0
size = 0
r = 0
g = 0
b = 0

# Callback function for the trackbar
def on_bold_trackbar(value):
    global bold
    bold = value
    
def on_size_trackbar(value):
    global size
    size = value
    
def on_r_trackbar(value):
    global r
    r = value

def on_g_trackbar(value):
    global g
    g = value

def on_b_trackbar(value):
    global b
    b = value
    
cv2.namedWindow("Camera")
cv2.createTrackbar("bold", "Camera", bold, 10, on_bold_trackbar)
cv2.createTrackbar("font size", "Camera", size, 10, on_size_trackbar)
cv2.createTrackbar("r", "Camera", r, 255, on_r_trackbar)
cv2.createTrackbar("g", "Camera", g, 255, on_g_trackbar)
cv2.createTrackbar("b", "Camera", b, 255, on_b_trackbar)

# 성공적으로 video device 가 열렸으면 while 문 반복
while(cap.isOpened()):
    # 한 프레임을 읽어옴
    ret, frame = cap.read()
    
    if ret is False:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    
    # Text
    cv2.putText(frame, "TEXT",
    topLeft, cv2.FONT_HERSHEY_SIMPLEX, size + 1, (r, g, b), 1 + bold)
    
    # Display
    cv2.imshow("Camera", frame)
     
    # Wait 1ms, if "q" exit
    key = cv2.waitKey(1)
    if key & 0xFF == ord("q"):
        break
    
cap.release()
cv2.destroyAllWindows()