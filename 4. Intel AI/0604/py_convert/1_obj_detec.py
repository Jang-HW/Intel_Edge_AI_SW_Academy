import cv2
import matplotlib.pyplot as plt
import numpy as np
import openvino as ov
from pathlib import Path

#notebook_utils 모듈 다운로드
import requests

r = requests.get(
    url="https://raw.githubusercontent.com/openvinotoolkit/openvino_notebooks/latest/utils/notebook_utils.py",
)

open("notebook_utils.py", "w").write(r.text)

from notebook_utils import download_file

base_model_dir = Path("./model").expanduser()

model_name = "horizontal-text-detection-0001"
model_xml_name = f"{model_name}.xml"
model_bin_name = f"{model_name}.bin"

model_xml_path = base_model_dir / model_xml_name
model_bin_path = base_model_dir / model_bin_name

if not model_xml_path.exists():
    model_xml_url = "https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.3/models_bin/1/horizontal-text-detection-0001/FP32/horizontal-text-detection-0001.xml"
    model_bin_url = "https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.3/models_bin/1/horizontal-text-detection-0001/FP32/horizontal-text-detection-0001.bin"

    download_file(model_xml_url, model_xml_name, base_model_dir)
    download_file(model_bin_url, model_bin_name, base_model_dir)
else:
    print(f"{model_name} 이미 {base_model_dir}에 다운로드되었습니다")

#장치 선택
core = ov.Core()
device = "AUTO"  # 필요에 따라 "CPU", "GPU" 등으로 변경 가능

#모델 로드
model = core.read_model(model=model_xml_path)
compiled_model = core.compile_model(model=model, device_name=device)

input_layer_ir = compiled_model.input(0)
output_layer_ir = compiled_model.output("boxes")

#이미지 다운로드
image_filename = download_file(
    "https://storage.openvinotoolkit.org/repositories/openvino_notebooks/data/data/image/intel_rnb.jpg",
    directory="data",
)

#이미지를 BGR 포맷으로 읽기
image = cv2.imread(str(image_filename))
#입력 크기 가져오기
N, C, H, W = input_layer_ir.shape

#이미지 크기 조정
resized_image = cv2.resize(image, (W, H))

#네트워크 입력 형태로 변환
input_image = np.expand_dims(resized_image.transpose(2, 0, 1), 0)

#추론 요청 생성
boxes = compiled_model([input_image])[output_layer_ir]

#0만 있는 박스 제거
boxes = boxes[~np.all(boxes == 0, axis=1)]

#결과를 이미지로 변환하는 함수
def convert_result_to_image(bgr_image, resized_image, boxes, threshold=0.3, conf_labels=True):
    colors = {"red": (255, 0, 0), "green": (0, 255, 0)}

    (real_y, real_x), (resized_y, resized_x) = (
        bgr_image.shape[:2],
        resized_image.shape[:2],
    )
    ratio_x, ratio_y = real_x / resized_x, real_y / resized_y

    # 이미지 BGR에서 RGB로 변환
    rgb_image = cv2.cvtColor(bgr_image, cv2.COLOR_BGR2RGB)

    for box in boxes:
        conf = box[-1]
        if conf > threshold:
            (x_min, y_min, x_max, y_max) = [
                (int(max(corner_position * ratio_y, 10)) if idx % 2 else int(corner_position * ratio_x)) for idx, corner_position in enumerate(box[:-1])
            ]

            rgb_image = cv2.rectangle(rgb_image, (x_min, y_min), (x_max, y_max), colors["green"], 3)

            if conf_labels:
                rgb_image = cv2.putText(
                    rgb_image,
                    f"{conf:.2f}",
                    (x_min, y_min - 10),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.8,
                    colors["red"],
                    1,
                    cv2.LINE_AA,
                )

    return rgb_image

#이미지 출력
plt.figure(figsize=(10, 6))
plt.axis("off")
plt.imshow(convert_result_to_image(image, resized_image, boxes, conf_labels=False))
plt.show()
