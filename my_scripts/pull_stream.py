import cv2
import time
import sys
sys.path.append("..")
from mineutils.path import Path


def pullStream():
    rtsp_url = 'rtmp://192.168.0.111:1935/live/home'
    ###ffmpeg -re -stream_loop -1 -i .\Normal_Fri_Aug_13_14_14_37_2021_Slomo.mp4 -f rtsp rtsp://192.168.74.1:8554/video
    cap = cv2.VideoCapture(rtsp_url)

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Opening camera is failed")
            break
        cv2.imshow("show", frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    

def pullStreamSave():
    rtsp_url = 'rtmp://192.168.0.111:1935/live/home'
    time_tp = time.localtime(time.time())
    save_path = f"E:/Datasets/AgriRobot_pull/ArgiRobot_{time_tp[0]:0>4}{time_tp[1]:0>2}{time_tp[2]:0>2}_{time_tp[3]:0>2}{time_tp[4]:0>2}{time_tp[5]:0>2}.avi"
    if not Path.exists(Path.parent(save_path)):
        Path.makeDirs(Path.parent(save_path))
    # save_path = time.strftime("%Y_%2m%2d_%2H:%2M:%2S", time.localtime())
    save_fps = 25
    ###ffmpeg -re -stream_loop -1 -i .\Normal_Fri_Aug_13_14_14_37_2021_Slomo.mp4 -f rtsp rtsp://192.168.74.1:8554/video
    cap = cv2.VideoCapture(rtsp_url)
    writer = cv2.VideoWriter()
    if cap.isOpened():
        writer.open(save_path, cv2.VideoWriter_fourcc('D', 'I', 'V', 'X'), save_fps,
                    (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))), True)
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Opening camera is failed")
            writer.release()
            break
        writer.write(frame)
        cv2.imshow("show", frame)
        if cv2.waitKey(1) & 0xFF == 27:
            writer.release()
            break

    cap.release()
    writer.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    pullStreamSave()
