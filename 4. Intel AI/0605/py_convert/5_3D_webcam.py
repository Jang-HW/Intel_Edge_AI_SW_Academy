import collections
import time
from pathlib import Path

import cv2
import ipywidgets as widgets
import numpy as np
from IPython.display import clear_output, display
import openvino as ov


# Fetch `notebook_utils` module
import requests

r = requests.get(
    url="https://raw.githubusercontent.com/openvinotoolkit/openvino_notebooks/latest/utils/notebook_utils.py",
)
with open("notebook_utils.py", "w") as f:
    f.write(r.text)

r = requests.get(
    url="https://raw.githubusercontent.com/openvinotoolkit/openvino_notebooks/latest/utils/engine3js.py",
)
with open("engine3js.py", "w") as f:
    f.write(r.text)

import notebook_utils as utils
import engine3js as engine

# directory where model will be downloaded
base_model_dir = "model"

# model name as named in Open Model Zoo
model_name = "human-pose-estimation-3d-0001"
# selected precision (FP32, FP16)
precision = "FP32"

BASE_MODEL_NAME = f"{base_model_dir}/public/{model_name}/{model_name}"
model_path = Path(BASE_MODEL_NAME).with_suffix(".pth")
onnx_path = Path(BASE_MODEL_NAME).with_suffix(".onnx")

ir_model_path = f"model/public/{model_name}/{precision}/{model_name}.xml"
model_weights_path = f"model/public/{model_name}/{precision}/{model_name}.bin"

core = ov.Core()

device = widgets.Dropdown(
    options=core.available_devices + ["AUTO"],
    value="AUTO",
    description="Device:",
    disabled=False,
)

device

# initialize inference engine
core = ov.Core()
# read the network and corresponding weights from file
model = core.read_model(model=ir_model_path, weights=model_weights_path)
# load the model on the specified device
compiled_model = core.compile_model(model=model, device_name=device.value)
infer_request = compiled_model.create_infer_request()
input_tensor_name = model.inputs[0].get_any_name()

# get input and output names of nodes
input_layer = compiled_model.input(0)
output_layers = list(compiled_model.outputs)

input_layer.any_name, [o.any_name for o in output_layers]

def model_infer(scaled_img, stride):
    """
    Run model inference on the input image

    Parameters:
        scaled_img: resized image according to the input size of the model
        stride: int, the stride of the window
    """

    # Remove excess space from the picture
    img = scaled_img[
        0 : scaled_img.shape[0] - (scaled_img.shape[0] % stride),
        0 : scaled_img.shape[1] - (scaled_img.shape[1] % stride),
    ]

    img = np.transpose(img, (2, 0, 1))[None,]
    infer_request.infer({input_tensor_name: img})
    # A set of three inference results is obtained
    results = {name: infer_request.get_tensor(name).data[:] for name in {"features", "heatmaps", "pafs"}}
    # Get the results
    results = (results["features"][0], results["heatmaps"][0], results["pafs"][0])

    return results

# 3D edge index array
body_edges = np.array(
    [
        [0, 1],
        [0, 9],
        [9, 10],
        [10, 11],  # neck - r_shoulder - r_elbow - r_wrist
        [0, 3],
        [3, 4],
        [4, 5],  # neck - l_shoulder - l_elbow - l_wrist
        [1, 15],
        [15, 16],  # nose - l_eye - l_ear
        [1, 17],
        [17, 18],  # nose - r_eye - r_ear
        [0, 6],
        [6, 7],
        [7, 8],  # neck - l_hip - l_knee - l_ankle
        [0, 12],
        [12, 13],
        [13, 14],  # neck - r_hip - r_knee - r_ankle
    ]
)


body_edges_2d = np.array(
    [
        [0, 1],  # neck - nose
        [1, 16],
        [16, 18],  # nose - l_eye - l_ear
        [1, 15],
        [15, 17],  # nose - r_eye - r_ear
        [0, 3],
        [3, 4],
        [4, 5],  # neck - l_shoulder - l_elbow - l_wrist
        [0, 9],
        [9, 10],
        [10, 11],  # neck - r_shoulder - r_elbow - r_wrist
        [0, 6],
        [6, 7],
        [7, 8],  # neck - l_hip - l_knee - l_ankle
        [0, 12],
        [12, 13],
        [13, 14],  # neck - r_hip - r_knee - r_ankle
    ]
)


def draw_poses(frame, poses_2d, scaled_img, use_popup):
    """
    Draw 2D pose overlays on the image to visualize estimated poses.
    Joints are drawn as circles and limbs are drawn as lines.

    :param frame: the input image
    :param poses_2d: array of human joint pairs
    """
    for pose in poses_2d:
        pose = np.array(pose[0:-1]).reshape((-1, 3)).transpose()
        was_found = pose[2] > 0

        pose[0], pose[1] = (
            pose[0] * frame.shape[1] / scaled_img.shape[1],
            pose[1] * frame.shape[0] / scaled_img.shape[0],
        )

        # Draw joints.
        for edge in body_edges_2d:
            if was_found[edge[0]] and was_found[edge[1]]:
                cv2.line(
                    frame,
                    tuple(pose[0:2, edge[0]].astype(np.int32)),
                    tuple(pose[0:2, edge[1]].astype(np.int32)),
                    (255, 255, 0),
                    4,
                    cv2.LINE_AA,
                )
        # Draw limbs.
        for kpt_id in range(pose.shape[1]):
            if pose[2, kpt_id] != -1:
                cv2.circle(
                    frame,
                    tuple(pose[0:2, kpt_id].astype(np.int32)),
                    3,
                    (0, 255, 255),
                    -1,
                    cv2.LINE_AA,
                )

    return frame

def run_pose_estimation(source=0, flip=False, use_popup=False, skip_frames=0):
    """
    2D image as input, using OpenVINO as inference backend,
    get joints 3D coordinates, and draw 3D human skeleton in the scene

    :param source:      The webcam number to feed the video stream with primary webcam set to "0", or the video path.
    :param flip:        To be used by VideoPlayer function for flipping capture image.
    :param use_popup:   False for showing encoded frames over this notebook, True for creating a popup window.
    :param skip_frames: Number of frames to skip at the beginning of the video.
    """

    focal_length = -1  # default
    stride = 8
    player = None
    skeleton_set = None

    try:
        # create video player to play with target fps  video_path
        # get the frame from camera
        # You can skip first N frames to fast forward video. change 'skip_first_frames'
        player = utils.VideoPlayer(source, flip=flip, fps=30, skip_first_frames=skip_frames)
        # start capturing
        player.start()

        input_image = player.next()
        # set the window size
        resize_scale = 450 / input_image.shape[1]
        windows_width = int(input_image.shape[1] * resize_scale)
        windows_height = int(input_image.shape[0] * resize_scale)

        # use visualization library
        engine3D = engine.Engine3js(grid=True, axis=True, view_width=windows_width, view_height=windows_height)

        if use_popup:
            # display the 3D human pose in this notebook, and origin frame in popup window
            display(engine3D.renderer)
            title = "Press ESC to Exit"
            cv2.namedWindow(title, cv2.WINDOW_KEEPRATIO | cv2.WINDOW_AUTOSIZE)
        else:
            # set the 2D image box, show both human pose and image in the notebook
            imgbox = widgets.Image(format="jpg", height=windows_height, width=windows_width)
            display(widgets.HBox([engine3D.renderer, imgbox]))

        skeleton = engine.Skeleton(body_edges=body_edges)

        processing_times = collections.deque()

        while True:
            # grab the frame
            frame = player.next()
            if frame is None:
                print("Source ended")
                break

            # resize image and change dims to fit neural network input
            # (see https://github.com/openvinotoolkit/open_model_zoo/tree/master/models/public/human-pose-estimation-3d-0001)
            scaled_img = cv2.resize(frame, dsize=(model.inputs[0].shape[3], model.inputs[0].shape[2]))

            if focal_length < 0:  # Focal length is unknown
                focal_length = np.float32(0.8 * scaled_img.shape[1])

            # inference start
            start_time = time.time()
            # get results
            inference_result = model_infer(scaled_img, stride)

            # inference stop
            stop_time = time.time()
            processing_times.append(stop_time - start_time)
            # Process the point to point coordinates of the data
            poses_3d, poses_2d = engine.parse_poses(inference_result, 1, stride, focal_length, True)

            # use processing times from last 200 frames
            if len(processing_times) > 200:
                processing_times.popleft()

            processing_time = np.mean(processing_times) * 1000
            fps = 1000 / processing_time

            if len(poses_3d) > 0:
                # From here, you can rotate the 3D point positions using the function "draw_poses",
                # or you can directly make the correct mapping below to properly display the object image on the screen
                poses_3d_copy = poses_3d.copy()
                x = poses_3d_copy[:, 0::4]
                y = poses_3d_copy[:, 1::4]
                z = poses_3d_copy[:, 2::4]
                poses_3d[:, 0::4], poses_3d[:, 1::4], poses_3d[:, 2::4] = (
                    -z + np.ones(poses_3d[:, 2::4].shape) * 200,
                    -y + np.ones(poses_3d[:, 2::4].shape) * 100,
                    -x,
                )

                poses_3d = poses_3d.reshape(poses_3d.shape[0], 19, -1)[:, :, 0:3]
                people = skeleton(poses_3d=poses_3d)

                try:
                    engine3D.scene_remove(skeleton_set)
                except Exception:
                    pass

                engine3D.scene_add(people)
                skeleton_set = people

                # draw 2D
                frame = draw_poses(frame, poses_2d, scaled_img, use_popup)

            else:
                try:
                    engine3D.scene_remove(skeleton_set)
                    skeleton_set = None
                except Exception:
                    pass

            cv2.putText(
                frame,
                f"Inference time: {processing_time:.1f}ms ({fps:.1f} FPS)",
                (10, 30),
                cv2.FONT_HERSHEY_COMPLEX,
                0.7,
                (0, 0, 255),
                1,
                cv2.LINE_AA,
            )

            if use_popup:
                cv2.imshow(title, frame)
                key = cv2.waitKey(1)
                # escape = 27, use ESC to exit
                if key == 27:
                    break
            else:
                # encode numpy array to jpg
                imgbox.value = cv2.imencode(
                    ".jpg",
                    frame,
                    params=[cv2.IMWRITE_JPEG_QUALITY, 90],
                )[1].tobytes()


    except KeyboardInterrupt:
        print("Interrupted")
    except RuntimeError as e:
        print(e)
    finally:
        clear_output()
        if player is not None:
            # stop capturing
            player.stop()
        if use_popup:
            cv2.destroyAllWindows()
        if skeleton_set:
            engine3D.scene_remove(skeleton_set)

USE_WEBCAM = True

cam_id = 0
# video_path = "https://github.com/intel-iot-devkit/sample-videos/raw/master/face-demographics-walking.mp4"

source = cam_id if USE_WEBCAM else video_path

run_pose_estimation(source=source, flip=isinstance(source, int), use_popup=True)