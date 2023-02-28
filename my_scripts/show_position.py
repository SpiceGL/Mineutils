# -*- coding: utf-8 -*-
"""
Created on Wed Aug 18 10:10:31 2021

@author: 00769901
"""
import cv2
from mineutils.colorstr import ColorStr as cs
from mineutils.draw import PixelMarker as PM
from mineutils import Error
from mineutils.path import Path


class PixelMarker:
    def __init__(self, title, img):
        self.title = title
        self.img = img
        self.window_left_upper = (192*2, 108*2)
        self.points = []
    
    def _draw_mark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            cv2.putText(img_drawn, str(xy), (x+10, y+10), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255), thickness=1)        
        cv2.imshow(self.title, img_drawn)
        
    def _onclick(self, event, x, y, flags, param):
        """
        Click on a point (x, y) will add this points to the list
        and re-draw the image.
        """
        if event == cv2.EVENT_LBUTTONDOWN:
            self.points.append((x, y))
            self._draw_mark()

    def start(self):
        cv2.namedWindow(self.title)
        cv2.moveWindow(self.title, self.window_left_upper[0], self.window_left_upper[1])
        cv2.setMouseCallback(self.title, self._onclick)
        cv2.imshow(self.title, self.img)
        while True:
            k = cv2.waitKey(0)
            if k & 0xff == 27 or k & 0xff == ord("q"):
                cv2.destroyAllWindows()             
                print(self.points)
                break
            if k & 0xff == ord("d"):
                self.points.pop()
                self._draw_mark()
            if k & 0xff == 13:          
                print(self.points)
                break


class BoxMarker(PM):
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.lines = []
    
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


def markPixel():
    cv2.destroyAllWindows()
    title = "NewType"
    img_path = "E:/chess.jpg"
    img = cv2.imread(img_path)
    myPixelMarker = PixelMarker(title, img)
    myPixelMarker.start()
    k = cv2.waitKey(0)
    # cv2.setMouseCallback(title, onclick, param=())
    
    if k & 0xff == 27 or k == ord("q"):
        cv2.destroyAllWindows()
        

def markRect():
    cv2.destroyAllWindows()
    title = "NewType"
    source_path = r"H:\Normal_Wed_Feb_22_16_19_25_2023.avi"   #img or video
    w, h = 720, 480
    cap = None
    if Path.isImage(source_path):
        img = cv2.imread(source_path)
    else:
        cap = cv2.VideoCapture(source_path)
        if not cap.isOpened():
            print(f"请检查{source_path}是否存在且为视频文件！\n")
            return
        ret, img = cap.read()
        if not ret:
            print(f"{ret} == false！\n")
            return
    cv2.resize(img, (w, h))
    myMarker = BoxMarker(title, img)
    myMarker.start()
    k = cv2.waitKey(0)
    # cv2.setMouseCallback(title, onclick, param=())
    
    if k & 0xff == 27 or k == ord("q"):
        cv2.destroyAllWindows()
    if cap:
        cap.release()


if __name__ == '__main__':
    markRect()









