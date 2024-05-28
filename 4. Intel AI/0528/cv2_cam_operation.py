import datetime
import cv2

# Read from the first cam
cap = cv2.VideoCapture(0)
fourcc = cv2.VideoWriter_fourcc(*'XVID')  # Using 'XVID' codec instead
record = False

w = 640        #1920
h = 360        #1080

cap.set(cv2.CAP_PROP_FRAME_WIDTH, w)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, h)

video = None  # Initialize video writer

# If opened video, while
while cap.isOpened():
    
    # 1 frame read
    ret, frame = cap.read()
    
    if not ret:
        print("Can't receive frame (stream end?) Exiting...")
        break

    now = datetime.datetime.now().strftime("%d_%H-%M-%S")

    # Display
    cv2.imshow("Camera", frame)

    # Wait 1ms, if "q" exit
    key = cv2.waitKey(1)
    if key & 0xFF == ord("q"):
        break
    
    elif key & 0xFF == ord("s"):
        print("녹화 시작")
        record = True
        video = cv2.VideoWriter("./" + str(now) + ".avi", fourcc, 20.0, (w, h))

    elif key & 0xFF == ord("e"):
        print("녹화 종료")
        record = False
        if video is not None:
            video.release()
            video = None

    if record and video is not None:
        video.write(frame)

cap.release()
cv2.destroyAllWindows()