import numpy as np
import cv2
import time

# Read video file
cap = cv2.VideoCapture("ronaldinho.mp4")

# if file opened, while prase
while (cap.isOpened()):
    # Read frame
    ret, frame = cap.read()

    if ret is False:
        cap = cv2.VideoCapture("ronaldinho.mp4")
        print("Can't receive frame (stream end) Restart!")
        #break
    else:
        # Display
        re_frame = frame[:, :720, :]
        cv2.imshow("Frame", re_frame)
        time.sleep(0.04)

    # wait while 1ms, input "q" exit
    key = cv2.waitKey(1)
    if key & 0xFF == ord("q"):
        break
        
    elif key & 0xFF == ord("c"):
        cv2.imwrite("output.png", frame)

cap.release()
cv2.destroyAllWindows()
