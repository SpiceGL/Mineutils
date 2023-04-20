# -*- coding: utf-8 -*-
"""
Created on Thu Aug 19 11:09:57 2021
用到的视频图像路径不要有中文！

@author: 00769901
"""

import os
import cv2
import sys
import random
sys.path.append("..")

from mineutils import Error
from mineutils.path import Path
from mineutils.quickshow import setWindowCV, loopShowCV, quickShowCV
from mineutils.draw import PixelMarker
from mineutils.colorstr import ColorStr as cs


def video2Pics(video_path, save_dir, show_frame=False, save_interval=0):
    Path.makeDirs(save_dir)
    window_name = "View"
    cap = cv2.VideoCapture(video_path)
    video_name = Path.splitName(video_path)
    current = 0
    
    setWindowCV(window_name, cv2.WINDOW_NORMAL)
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            cap.release()
            cv2.destroyWindow(window_name)
            break
        current += 1
        
        if show_frame:
            cv2.imshow(window_name, frame)
            k = cv2.waitKey(1)
            loopShowCV(window_name, frame, wait=1)
        save_name = video_name + "_" + str(current).zfill(5) + ".jpg"
        if save_interval:
            if current % save_interval == 0:
                cv2.imwrite(os.path.join(save_dir, save_name), frame)
        else:
            cv2.imwrite(os.path.join(save_dir, save_name), frame)
    cap.release()
    cv2.destroyAllWindows()
    

def main_video2Pics():
    cv2.destroyAllWindows()
    video_path = "../Videos/1027/change_angle/Normal_Sat_Oct_23_16_06_00_2021.avi"
    save_dir = "../Datas/change_angle"
    video2Pics(video_path, save_dir, show_frame=True, save_interval=20)
    
   
class BoxMarker(PixelMarker):
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.lines = []
        
    def _setTextPosition(self, point):
        return point

    def _drawMark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            text_x, text_y = self._setTextPosition((x, y))
            cv2.putText(img_drawn, str(xy), (text_x, text_y), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255),
                        thickness=1)
        for line in self.lines:
            cv2.line(img_drawn, line[0], line[1], (0, 255, 0), thickness=1)
        cv2.imshow(self.title, img_drawn)
    
    def _onclick(self, event, x, y, flags, param):
        """
        --鼠标在图像上的点击操作。
        """
        if event == cv2.EVENT_LBUTTONDOWN:
            if len(self.points) == 0:
                self.points.append((x, y))
                self._drawMark()
            elif len(self.points) <= 2:
                if len(self.points) == 2:
                    self.points.pop()
                    self.lines = []
                self.points.append((x, y))
                l, t = self.points[0]
                r, b = self.points[1]
                self.lines.append([(l, t), (l, b)])
                self.lines.append([(l, t), (r, t)])
                self.lines.append([(l, b), (r, b)])
                self.lines.append([(r, t), (r, b)])
                self._drawMark()
                print(cs.green("BoxMarker:"), "裁剪框已完成，如需修改继续点击修改最近点，或按'd'删除所有点。如确认无误请按'Enter'键继续！")
        
    def start(self):
        cv2.namedWindow(self.title, cv2.WINDOW_AUTOSIZE)
        # cv2.resize(self.title, self.window_size[0], self.window_size[1])
        cv2.moveWindow(self.title, self.position[0], self.position[1])
        cv2.setMouseCallback(self.title, self._onclick)
        cv2.imshow(self.title, self.img)
        while True:
            k = cv2.waitKey(0)
            if k & 0xff == 27:
                cv2.destroyAllWindows()
                raise Error("BoxMarker.start(): 用户主动中止！")
            if k & 0xff == ord("d"):
                self.points = []
                self.lines = []
                self._drawMark()
            if k & 0xff == 13:
                if len(self.points) < 2:
                    print(cs.yellow("BoxMarker:"), "裁剪框未设置完成，请继续绘制对角点！")
                else:
                    break
        print(cs.blue("BoxMarker.start()返回的点:"), self.points)
        return self.points


def random_cutsize(max_size):
    res = 2 * random.random() - 1
    res *= max_size
    return res


def windingVideoCut(video_path_or_dir, save_fdir, save_fps=25, show_saved_frame=False):
    Path.makeDirs(save_fdir)
    window_name = "View"

    if Path.isFile(video_path_or_dir):
        video_paths = [video_path_or_dir]
    else:
        video_paths = Path.listDir(video_path_or_dir)

    for vpath in video_paths:
        cap = cv2.VideoCapture(vpath)
        video_name = Path.splitName(vpath, False)
        Path.makeDirs(save_fdir)
        current = 0
        
        
        
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                cap.release()
                cv2.destroyWindow(window_name)
                break
            if current == 0:
                box_marker = BoxMarker("Box Mark", frame)
                pt0, pt1 = box_marker.start()

                save_name = "Cut_" + video_name + ".avi"
                save_path = os.path.join(save_dir, save_name)
                writer = cv2.VideoWriter(save_path, cv2.VideoWriter_fourcc('D', 'I', 'V', 'X'), save_fps,
                                         (abs(pt1[0] - pt0[0]), abs(pt1[1] - pt0[1])))
                l = min(pt0[0], pt1[0])
                t = min(pt0[1], pt1[1])
                r = max(pt0[0], pt1[0])
                b = max(pt0[1], pt1[1])
            current += 1
            
            

            saved_img = frame[t:b, l:r]
            if show_saved_frame:
                k = quickShowCV(window_name, saved_img, wait=1)
                writer.write(saved_img)
        cap.release()
        cv2.destroyAllWindows()


def windingVideoCut2Img(video_path_or_dir, save_dir, save_interval=0, show_saved_frame=False, save_prefix=""):
    rand_cut_rate = 0.1   ###上下随机裁剪比例
    
    Path.makeDirs(save_dir)
    window_name = "View"
    
    if Path.isFile(video_path_or_dir):
        video_paths = [video_path_or_dir]
    else:
        video_paths = Path.listDir(video_path_or_dir)
    
    for vpath in video_paths:
        cap = cv2.VideoCapture(vpath)
        video_name = Path.splitName(vpath, False)
        current = 0
        
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                cap.release()
                cv2.destroyWindow(window_name)
                break
            if current == 0:
                box_marker = BoxMarker(video_name, frame)
                pt0, pt1 = box_marker.start()
            current += 1
            
            save_name = save_prefix + video_name + "_" + str(current).zfill(5) + ".jpg"
            save_path = os.path.join(save_dir, save_name)

            l = min(pt0[0], pt1[0])
            t = min(pt0[1], pt1[1])
            r = max(pt0[0], pt1[0])
            b = max(pt0[1], pt1[1])

            max_cut_size = rand_cut_rate * abs(b - t)
            l = round(max(0, l + random_cutsize(max_cut_size)))
            t = round(max(0, t + random_cutsize(max_cut_size)))
            r = round(min(frame.shape[1], r + random_cutsize(max_cut_size)))
            b = round(min(frame.shape[0], b + random_cutsize(max_cut_size)))
            
            if save_interval:
                if current % save_interval == 1:
                    
                    saved_img = frame[t:b, l:r]
                    if show_saved_frame:
                        k = quickShowCV(window_name, saved_img, wait=1)
                    cv2.imwrite(save_path, saved_img)
            else:
                saved_img = frame[t:b, l:r]
                if show_saved_frame:
                    k = quickShowCV(window_name, saved_img, wait=1)
                cv2.imwrite(save_path, saved_img)
        cap.release()
        cv2.destroyAllWindows()
        
        


if __name__ == '__main__':
    video_dir = r"E:\Datasets\Z106\test_video\normal"
    save_dir = r"E:\Datasets\Z106\Cut\test/normal"
    # Path.remove(save_dir)
    # windingVideoCut(video_dir, save_dir, 25, True)
    windingVideoCut2Img(video_dir, save_dir, 50, True, "Z106_")
