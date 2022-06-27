import cv2
import sys
sys.path.append("..")
from mineutils.quickshow import setWindowCV, quickShowCV, loopShowCV


if __name__ == "__main__":
    rtsp_url = 'rtsp://192.168.1.128:8554/video'
    ###ffmpeg -re -stream_loop -1 -i .\Normal_Fri_Aug_13_14_14_37_2021_Slomo.mp4 -f rtsp rtsp://192.168.74.1:8554/video
    cap = cv2.VideoCapture(rtsp_url)

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Opening camera is failed")
            break
        quickShowCV(rtsp_url, frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    