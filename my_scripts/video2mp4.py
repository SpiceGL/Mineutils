import cv2


def main_video2mp4():
    source_path = r"E:\Datasets\AgriRobot_pull\20230410\ArgiRobot_20230410_163814.avi"
    save_path = r"E:\Datasets\AgriRobot_pull\20230410\ArgiRobot_20230410_163814.mp4"
    
    cap = cv2.VideoCapture()
    cap.open(source_path)
    if not cap.isOpened():
        print(f"{source_path} open failed!")
        return
    writer = cv2.VideoWriter()
    writer.open(save_path, cv2.VideoWriter_fourcc("m", "p", "4", "v"), cap.get(cv2.CAP_PROP_FPS),
                (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))))
    if not writer.isOpened():
        print(f"{save_path} open failed!")
        return
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        cv2.imshow("show", frame)
        cv2.waitKey(1)
        writer.write(frame)
    cap.release()
    writer.release()

if __name__ == '__main__':
    main_video2mp4()