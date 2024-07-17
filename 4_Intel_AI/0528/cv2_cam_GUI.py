import cv2

# read from the first cam
cap = cv2.VideoCapture(0)

topLeft = (50, 50)
bottomRight = (300, 300)
click_x = 0
click_y = 0

def onMouse(event, x, y, flags, param) :
    global click_x, click_y
    if event == cv2.EVENT_LBUTTONDOWN :
        print('클릭 좌표 : ', x, y)
        click_x = x
        click_y = y

cv2.namedWindow("Camera")
cv2.setMouseCallback("Camera", onMouse)

# 성공적으로 video device 가 열렸으면 while 문 반복
while (cap.isOpened()):
    # 한 프레임을 읽어옴
    ret, frame = cap.read()
    
    # Line
    cv2.line(frame, topLeft, bottomRight, (0, 255, 0), 5)
    
    # Rectangle
    cv2.rectangle(frame, 
                  [pt+30 for pt in topLeft], [pt-30 for pt in bottomRight], 
                  (0, 0, 255), 5)
    
    # Text
    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(frame, 'me', [pt+80 for pt in topLeft], font, 2, (0, 0, 0), 5)
    
    # Circle
    cv2.circle(frame, (300, 300), 60, (255, 255, 0), 5, cv2.LINE_AA)
    cv2.circle(frame, (click_x, click_y), 2, (255, 255, 0), 5, cv2.LINE_AA)
    
    # Display
    cv2.imshow("Camera", frame)
 
    # Wait 1ms, if "q" exit
    key = cv2.waitKey(1)
    if key & 0xFF == ord("q"):
        break
    
cap.release()
cv2.destroyAllWindows()
