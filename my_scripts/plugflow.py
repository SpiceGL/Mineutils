import cv2
import time
import subprocess as sp
import multiprocessing
import sys
sys.path.append("..")
from mineutils.quickshow import setWindowCV, quickShowCV, loopShowCV


class stream_pusher(object):
    def __init__(self, rtmp_url=None, raw_frame_q=None):  # 类实例化的时候传入rtmp地址和帧传入队列
        self.rtmp_url = rtmp_url
        self.raw_frame_q = raw_frame_q

        fps = 20  # 设置帧速率
        # 设置分辨率
        width = 1920  # 宽
        height = 1080  # 高

        # 设置FFmpeg命令文本
        self.command = ['ffmpeg',
                        '-y',
                        '-f', 'rawvideo',
                        '-vcodec', 'rawvideo',
                        '-pix_fmt', 'bgr24',
                        '-s', "{}x{}".format(width, height),
                        '-r', str(fps),
                        '-i', '-',
                        '-c:v', 'libx264',
                        '-pix_fmt', 'yuv420p',
                        '-preset', 'ultrafast',
                        '-f', 'flv',
                        self.rtmp_url]

    # 向服务器推送
    def push_frame(self):

        # 配置向os传递命令的管道
        p = sp.Popen(self.command, stdin=sp.PIPE)

        while True:
            if not self.raw_frame_q.empty():  # 如果输入管道不为空
                # 把帧和相关信息从输入队列中取出
                frame = self.raw_frame_q.get()

                # 把内容放入管道，放入后有os自己去执行
                p.stdin.write(frame.tostring())
            else:
                time.sleep(0.01)

    # 启动运行
    def run(self):
        # 定义一个子进程
        push_frame_p = multiprocessing.Process(target=self.push_frame, args=())
        push_frame_p.daemon = True  # 把子进程设置为daemon方式
        push_frame_p.start()  # 运行子进程
        
        
class RTSPPusher:
    def __init__(self, dst_url, cap_id):
        self.dst_url = dst_url
        self.cap_id = cap_id
        
    def run(self):
        cap = cv2.VideoCapture(self.cap_id)
        fps = int(cap.get(cv2.CAP_PROP_FPS))
        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
        command = ['ffmpeg',
                   '-y',
                   '-f', 'rawvideo',
                   '-vcodec', 'rawvideo',
                   '-pix_fmt', 'bgr24',
                   '-s', "{}x{}".format(width, height),
                   '-r', str(fps),
                   '-i', '-',
                   '-c:v', 'libx264',
                   '-pix_fmt', 'yuv420p',
                   '-preset', 'ultrafast',
                   # '-rtsp_transport', "tcp",
                   '-f', 'rtsp',
                   self.dst_url]
        p = sp.Popen(command, stdin=sp.PIPE)
        win_name = setWindowCV("pusher")
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                print("Opening camera is failed")
                break
            still_loop = loopShowCV(win_name, frame)
            if not still_loop:
                cv2.destroyWindow(win_name)
                break
            p.stdin.write(frame.tostring())
        cap.release()
        cv2.destroyAllWindows()


class Puller:
    def __init__(self, url):
        self.url = url
        
    def run(self):
        cap = cv2.VideoCapture(self.url)
        cv2.namedWindow("puller", cv2.WINDOW_AUTOSIZE)
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                print("Open url failed")
                break
            cv2.imshow("puller", frame)
            k = cv2.waitKey(1) & 0xff
            if k == 27 or k == ord('q'):
                break
        cap.release()
        cv2.destroyAllWindows()


if __name__ == '__main__':

    # cap = cv2.VideoCapture(r"E:\Project\002-卷扬乱绳识别\Videos\Normal_Fri_Aug_13_14_14_37_2021_Slomo.mp4")
    #
    # rtmpUrl = "rtmp://192.168.1.11/video"  # 用vcl等直播软件播放时，也用这个地址
    # raw_q = multiprocessing.Queue()  # 定义一个向推流对象传入帧及其他信息的队列
    #
    # my_pusher = stream_pusher(rtmp_url=rtmpUrl, raw_frame_q=raw_q)  # 实例化一个对象
    # my_pusher.run()  # 让这个对象在后台推送视频流
    # while True:;
    #     _, raw_frame = cap.read()
    #
    #     if not raw_q.full():  # 如果队列没满
    #         raw_q.put(raw_frame)  # 送入队列
    #         quickShowCV("raw_frame", raw_frame)
    #     cv2.waitKey(1)

    rtsp_url = 'rtsp://192.168.1.11:8554/video'
    rtsppusher = RTSPPusher(rtsp_url, 'rtsp://192.168.1.13:8554/live1')
    rtsppusher.run()

    ###ffmpeg -re -stream_loop -1 -i .\Normal_Fri_Aug_13_14_14_37_2021_Slomo.mp4 -f rtsp rtsp://192.168.74.1:8554/video

    