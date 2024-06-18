import cv2

def get_camera_indices(max_devices=10):
    available_indices = []
    for i in range(max_devices):
        cap = cv2.VideoCapture(i)
        if cap.isOpened():
            available_indices.append(i)
            cap.release()
    return available_indices

#카메라 인덱스 확인
camera_indices = get_camera_indices()
print("Available camera indices:", camera_indices)