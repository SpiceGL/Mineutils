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

import numpy as np

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
    
   
class BoxMarker(PixelMarker):
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.lines = []
        self.thickness = int(np.ceil(max(img.shape[0] / window_size[1], img.shape[1] / window_size[0])))
        
    def _setTextPosition(self, point):
        return point

    def _drawMark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            text_x, text_y = self._setTextPosition((x, y))
            cv2.putText(img_drawn, str(xy), (text_x, text_y), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255),
                        thickness=self.thickness)
        for line in self.lines:
            cv2.line(img_drawn, line[0], line[1], (0, 255, 0), thickness=self.thickness)
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
        
    def start(self, ltrb=None):
        cv2.namedWindow(self.title, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(self.title, self.window_size[0], self.window_size[1])
        cv2.moveWindow(self.title, self.position[0], self.position[1])
        cv2.setMouseCallback(self.title, self._onclick)
        if ltrb is not None:
            self.points = []
            self.lines = []
            self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[0], ltrb[1], None, None)
            self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[2], ltrb[3], None, None)
        else:
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
                save_path = os.path.join(save_fdir, save_name)
                writer = cv2.VideoWriter(save_path, cv2.VideoWriter_fourcc('D', 'I', 'V', 'X'), save_fps,
                                         (abs(pt1[0] - pt0[0]), abs(pt1[1] - pt0[1])))
                l = min(pt0[0], pt1[0])
                t = min(pt0[1], pt1[1])
                r = max(pt0[0], pt1[0])
                b = max(pt0[1], pt1[1])
            current += 1
            
            

            saved_img = frame[t:b, l:r]
            if show_saved_frame:
                quickShowCV(window_name, saved_img, wait=1)
                writer.write(saved_img)
        cap.release()
        cv2.destroyAllWindows()


def windingVideoCut2Img(video_path_or_dir, save_dir, save_interval=1, show_saved_frame=False, save_prefix="", border_radio=0.1):
    assert border_radio >= 0
    assert save_interval >= 1
    save_interval = int(save_interval)
    
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

            if current % save_interval == 0:
                save_name = save_prefix + video_name + "_" + str(current).zfill(5) + ".jpg"
                save_path = os.path.join(save_dir, save_name)
    
                l = min(pt0[0], pt1[0])
                t = min(pt0[1], pt1[1])
                r = max(pt0[0], pt1[0])
                b = max(pt0[1], pt1[1])
                
                l = round(l - ((r - l) * border_radio / 2.0))
                t = round(t - ((b - t) * border_radio / 2.0))
                r = round(r + ((r - l) * border_radio / 2.0))
                b = round(b + ((b - t) * border_radio / 2.0))
    
                # rand_cut_rate = 0.1  ###上下随机裁剪比例
                # max_cut_size = rand_cut_rate * abs(b - t)
                # l = round(max(0, l + random_cutsize(max_cut_size)))
                # t = round(max(0, t + random_cutsize(max_cut_size)))
                # r = round(min(frame.shape[1], r + random_cutsize(max_cut_size)))
                # b = round(min(frame.shape[0], b + random_cutsize(max_cut_size)))
                
                l_border = 0 if l >= 0 else (0 - l)
                t_border = 0 if t >= 0 else (0 - t)
                r_border = 0 if r <= frame.shape[1] else r - frame.shape[1]
                b_border = 0 if b <= frame.shape[0] else b - frame.shape[0]
                
                l = l if l >= 0 else 0
                t = t if t >= 0 else 0
                r = r if r <= frame.shape[1] else frame.shape[1]
                b = b if b <= frame.shape[0] else frame.shape[0]
                
                saved_img = frame[t:b, l:r]
                saved_img = cv2.copyMakeBorder(saved_img, t_border, b_border, l_border, r_border, cv2.BORDER_CONSTANT, value=(0, 0, 0))
                if show_saved_frame:
                    quickShowCV(window_name, saved_img, wait=1)
                cv2.imwrite(save_path, saved_img)
                print(cs.green("windingVideoCut2Img:"), f"已保存到{save_path}!")
            current += 1
        cap.release()
        cv2.destroyAllWindows()
        
        
def surveillanceCamera2Pics(src_path, save_dir, save_interval, overlap_area_radio=0.3):
    if not Path.exists(save_dir):
        Path.makeDirs(save_dir)
    
    if Path.isVideo(src_path):
        video_paths = [src_path]
    elif Path.isDir(src_path):
        video_paths = Path.listDir(src_path, return_path=True)
    else:
        print("请输入正确的src_path，视频路径或视频所在目录！")
        return
    
    ltrb = None
    for vpath in video_paths:
        cap = cv2.VideoCapture(vpath)
        video_name = Path.splitName(vpath, suffix=False)
        if not cap.isOpened():
            print(f"打开视频{vpath}失败，已跳过！")
            continue
        current = 0
        while True:
            ret, frame = cap.read()
            if not ret:
                print(f"读取视频{vpath}结束，准备读取下一个视频……")
                break
            if current == 0:
                box_marker = BoxMarker(video_name, frame, (1280, 720))
                pt0, pt1 = box_marker.start(ltrb)
            
            
            if current % save_interval == 0:
                
    
                l = min(pt0[0], pt1[0])
                t = min(pt0[1], pt1[1])
                r = max(pt0[0], pt1[0])
                b = max(pt0[1], pt1[1])
                ltrb = [l, t, r, b]
                crop_height = b - t + 1
                crop_weight = int(crop_height * 1.5)
                crop_l = l
                while crop_l <= r:
                    crop_r = crop_l + crop_weight - 1
                    if crop_r - r >= crop_weight / 2:
                        break
                    save_img = frame[t: b+1, crop_l: crop_r+1]
                    save_name = video_name + "_" + str(current).zfill(5) + f"_{crop_l}_{crop_r}" + ".jpg"
                    save_path = os.path.join(save_dir, save_name)
                    cv2.imwrite(save_path, save_img)
                    print(f"已保存到{save_path}!")
                    # cv2.imshow(f"_{crop_l}_{crop_r}", save_img)
                    # k = cv2.waitKey(1)
                    # if k & 0xff == 27:
                    #     print(cs.red("surveillanceCamera2Pics:"), f"用户主动退出程序！")
                    #     exit(0)
                    crop_l = crop_l + int(crop_weight*(1-overlap_area_radio))
                    print("crop_l =", crop_l)

            current += 1


class MultiBoxMarker(PixelMarker):
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.lines = []
        self.now_boxid = 0
        self.locked = False
        self.thickness = int(np.ceil(max(img.shape[0] / window_size[1], img.shape[1] / window_size[0])))
    
    def _setTextPosition(self, point):
        return point
    
    def _drawMark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            text_x, text_y = self._setTextPosition((x, y))
            cv2.putText(img_drawn, str(xy), (text_x, text_y), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255),
                        thickness=self.thickness)
        for line in self.lines[0:4*self.now_boxid]:
            cv2.line(img_drawn, line[0], line[1], (114, 114, 114), thickness=self.thickness)
        for line in self.lines[4*self.now_boxid:]:
            cv2.line(img_drawn, line[0], line[1], (0, 255, 0), thickness=self.thickness)
        cv2.imshow(self.title, img_drawn)
    
    def _onclick(self, event, x, y, flags, param):
        """
        --鼠标在图像上的点击操作。
        """
        if not self.locked:
            if event == cv2.EVENT_LBUTTONDOWN:
                if len(self.points) - 2*self.now_boxid == 0:
                    self.points.append((x, y))
                    self._drawMark()
                elif len(self.points) - 2*self.now_boxid <= 2:
                    if len(self.points) - 2*self.now_boxid == 2:
                        self.points.pop()
                        self.lines = self.lines[0:-4]
                    self.points.append((x, y))
                    l, t = self.points[-2]
                    r, b = self.points[-1]
                    self.lines.append([(l, t), (l, b)])
                    self.lines.append([(l, t), (r, t)])
                    self.lines.append([(l, b), (r, b)])
                    self.lines.append([(r, t), (r, b)])
                    self._drawMark()
                    print(cs.green("BoxMarker:"), "当前裁剪框已完成，如需修改继续点击修改最近点，或按'd'删除当前裁剪框，或按'Backspace'键删除所有框。如确认无误请按'空格'键继续绘制，或按'Enter'键结束绘制！")
    
    def start(self, ltrb_list=None):
        cv2.namedWindow(self.title, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(self.title, self.window_size[0], self.window_size[1])
        cv2.moveWindow(self.title, self.position[0], self.position[1])
        cv2.setMouseCallback(self.title, self._onclick)
        if ltrb_list is not None:
            self.points = []
            self.lines = []
            for ltrb in ltrb_list:
                self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[0], ltrb[1], None, None)
                self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[2], ltrb[3], None, None)
        else:
            cv2.imshow(self.title, self.img)
        while True:
            k = cv2.waitKey(0)
            if k & 0xff == 27:
                cv2.destroyAllWindows()
                raise Error("BoxMarker.start(): 用户主动中止！")
            if k & 0xff == ord("d"):
                self.points = self.points[0:2*self.now_boxid]
                self.lines = self.lines[0:4*self.now_boxid]
                # if self.now_boxid >= 1:
                #     self.now_boxid -= 1
                # else:
                #     self.now_boxid = 0
                self._drawMark()
            if k & 0xff == 8:
                self.points = []
                self.lines = []
                self.now_boxid = 0
                self._drawMark()
            if k & 0xff == 32:
                if len(self.points) - 2*self.now_boxid == 0:
                    print(cs.yellow("BoxMarker:"), "当前裁剪框未开始绘制，请继续绘制对角点，或按'Enter'键结束绘制！")
                elif len(self.points) - 2*self.now_boxid == 1:
                    print(cs.yellow("BoxMarker:"), "裁剪框未设置完成，请继续绘制对角点！")
                else:
                    self.now_boxid += 1
                    print(cs.blue("BoxMarker:"), "开始绘制下一个裁剪框。")
                    self._drawMark()
            if k & 0xff == 13:
                if len(self.points) - 2*self.now_boxid == 1 or len(self.points) == 0:
                    print(cs.yellow("BoxMarker:"), "裁剪框未设置完成，请继续绘制对角点！")
                else:
                    self.now_boxid += 1
                    self._drawMark()
                    self.locked = True
                    break
        print(cs.blue("BoxMarker.start()返回的点:"), self.points)
        return self.points



def windingVideoZhuqiCut2Img(video_path_or_dir, save_dir, save_interval=1, angle=45, save_prefix="",
                             border_radio=0.1):
    """ 株洲起重机公司的卷扬图像
    :param video_path_or_dir: 视频路径或存放视频的目录
    :param save_dir: 裁剪图像保存目录
    :param save_interval: 多少帧保存一次
    :param angle: 帧图像旋转角度，逆时针为正
    :param save_prefix: 保存图像名添加的前缀
    :param border_radio: 选定图像区域后，添加边缘的比例，>=0
    :return: None
    """
    assert border_radio >= 0
    assert save_interval >= 1
    save_interval = int(save_interval)
    
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
            w, h = frame.shape[1], frame.shape[0]
            length_new = round(np.sqrt(np.square(frame.shape[0]) + np.square(frame.shape[1])))
            w_new, h_new = length_new, length_new
            if (w_new - w) % 2 != 0:
                w_new += 1
            if (h_new - h) % 2 != 0:
                h_new += 1
            frame = cv2.copyMakeBorder(frame, (h_new - h) // 2, (h_new - h) // 2, (w_new - w) // 2, (w_new - w) // 2,
                                       cv2.BORDER_CONSTANT, value=(0, 0, 0))
            warp_mat = cv2.getRotationMatrix2D((w_new / 2, h_new / 2), angle, 1.0)
            frame = cv2.warpAffine(frame, warp_mat, (w_new, h_new))
            
            if current == 0:
                box_marker = MultiBoxMarker(video_name, frame, (720, 720))
                pts = box_marker.start()
            
            group_id = 0
            while current % save_interval == 0 and group_id < len(pts)//2:
                pt0 = pts[2*group_id]
                pt1 = pts[2*group_id+1]
                save_name = save_prefix + str(group_id).zfill(2) + "_" + video_name + "_" + str(current).zfill(5) + ".jpg"
                save_path = os.path.join(save_dir, save_name)
                
                l = min(pt0[0], pt1[0])
                t = min(pt0[1], pt1[1])
                r = max(pt0[0], pt1[0])
                b = max(pt0[1], pt1[1])
                
                l = round(l - ((r - l) * border_radio / 2.0))
                t = round(t - ((b - t) * border_radio / 2.0))
                r = round(r + ((r - l) * border_radio / 2.0))
                b = round(b + ((b - t) * border_radio / 2.0))
                
                l_border = 0 if l >= 0 else (0 - l)
                t_border = 0 if t >= 0 else (0 - t)
                r_border = 0 if r <= frame.shape[1] else r - frame.shape[1]
                b_border = 0 if b <= frame.shape[0] else b - frame.shape[0]
                
                l = l if l >= 0 else 0
                t = t if t >= 0 else 0
                r = r if r <= frame.shape[1] else frame.shape[1]
                b = b if b <= frame.shape[0] else frame.shape[0]
                
                saved_img = frame[t:b, l:r]
                saved_img = cv2.copyMakeBorder(saved_img, t_border, b_border, l_border, r_border, cv2.BORDER_CONSTANT,
                                               value=(0, 0, 0))
                quickShowCV(window_name, saved_img, wait=1)
                cv2.imwrite(save_path, saved_img)
                print(cs.green("windingVideoCut2Img:"), f"已保存到{save_path}!")
                group_id += 1
            current += 1
        cap.release()
        cv2.destroyAllWindows()
            
        

def main_video2Pics():
    cv2.destroyAllWindows()
    video_path = "../Videos/1027/change_angle/Normal_Sat_Oct_23_16_06_00_2021.avi"
    save_dir = "../Datas/change_angle"
    video2Pics(video_path, save_dir, show_frame=True, save_interval=20)
    
    
def main_windingVideoCut2Img():
    video_dir = r"E:\Datasets\ZTC1000V663-1\ZTC1000V663-1_230301\Wrong"
    save_dir = r"E:\Datasets\ZTC1000V663-1\ZTC1000V663-1_230301_Cut230613\Wrong"
    # Path.remove(save_dir)
    windingVideoCut2Img(video_dir, save_dir, 30, True, "Z106_", 0.1)


def main_surveillanceCamera2Pics():
    video_dir = r"E:\Datasets\AgriRobot_pull\from_surveillance_camera"
    save_dir = r"E:\Datasets\AgriRobot_pull\from_surveillance_camera\crop_saved"
    surveillanceCamera2Pics(video_dir, save_dir, 30, 0.3)
    

def main_windingVideoZhuqiCut2Img():
    video_dir = r"E:\Datasets\ZTC1000V663-1\test_videos"
    save_dir = r"E:\Datasets\ZTC1000V663-1\test_videos\save_test"
    # Path.remove(save_dir)
    windingVideoZhuqiCut2Img(video_dir, save_dir, 30, angle=45, save_prefix="Zhuqi_", border_radio=0.1)


if __name__ == '__main__':
    main_windingVideoZhuqiCut2Img()
