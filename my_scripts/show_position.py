# -*- coding: utf-8 -*-
"""
Created on Wed Aug 18 10:10:31 2021

@author: 00769901
"""
import cv2
from mineutils.colorstr import ColorStr as cs
from mineutils.draw import PixelMarker as PM
from mineutils.draw import putBbox
from mineutils import Error
from mineutils.path import Path


class BoxMarker(PM):
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.lines = []
    
    def _drawMark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            cv2.putText(img_drawn, str(xy), (x, y), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255),
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
                print(cs.green("BoxMarker:"), "矩形框已完成，如需修改继续点击修改最近点，或按'd'删除所有点。如确认无误请按'Enter'键继续！")
    
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
                    print(cs.yellow("BoxMarker:"), "矩形框未设置完成，请继续绘制对角点！")
                else:
                    break
        print(cs.blue("BoxMarker.start()返回的点:"), self.points)
        return self.points


###用于手动在图像上绘制带标签的检测框
class LabelMarker(PM):
    def __init__(self, title, img, label, color, window_size=(720, 480), position=(0, 0)):
        super().__init__(title, img, window_size, position)
        self.img = img
        self.res_img = img.copy()
        self.label = label
        self.color = color
    
    def _drawMark(self):
        if len(self.points) == 0:
            img_drawn = self.img.copy()
            self.res_img = self.img.copy()
        else:
            img_drawn = self.res_img.copy()
            
        if len(self.points) == 0:
            pass
        elif len(self.points) % 2 == 1:
            xy = self.points[-1]
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
        elif len(self.points) % 2 == 0:
            l, t = self.points[-2][0], self.points[-2][1]
            r, b = self.points[-1][0], self.points[-1][1]
            putBbox(img_drawn, [l,t,r,b], self.label, None, self.color)
            self.res_img = img_drawn
        cv2.imshow(self.title, img_drawn)
    
    def _onclick(self, event, x, y, flags, param):
        """
        --鼠标在图像上的点击操作。
        """
        if event == cv2.EVENT_LBUTTONDOWN:
            self.points.append((x, y))
            self._drawMark()
    
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
                raise Error("LabelMarker.start(): 用户主动中止！")
            if k & 0xff == ord("d"):
                self.points = []
                self._drawMark()
            if k & 0xff == 13:
                print("绘制下个标签")
                break
        cv2.destroyWindow(self.title)
        return self.res_img


def mainMarkPixel():
    cv2.destroyAllWindows()
    title = "NewType"
    img_path = "E:/chess.jpg"
    img = cv2.imread(img_path)
    myPixelMarker = PM(title, img)
    myPixelMarker.start()
    

###标出矩形框和其对应的左上角与右下角坐标
def mainMarkRect():
    cv2.destroyAllWindows()
    title = "NewType"
    source_path = r"E:\Datasets\coor\ZTC1000V663-1\ZTC1000V663-1_LP207_Normal_Fri_Feb_17_10_55_06_2023.avi"   #img or video
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
    # cv2.resize(img, (w, h))
    myMarker = BoxMarker(title, img)
    myMarker.start()
    k = cv2.waitKey(0)
    # cv2.setMouseCallback(title, onclick, param=())
    
    if k & 0xff == 27 or k == ord("q"):
        cv2.destroyAllWindows()
    if cap:
        cap.release()


###手动画检测框，结果保存为输入图像同一级目录下的mainMrawLabel.jpg
def mainMrakLabel():
    img_path = r"E:\00005.jpg"
    save_path = Path.join(Path.parent(img_path, True), "mainMrawLabel.jpg")
    img = cv2.imread(img_path)
    labels = ["crop", "weed", "OutOfRange"]
    colors = [[0, 255, 0], [0, 0, 255], [255, 0, 0]]
    img_res = img.copy()
    for i in range(len(labels)):
        marker = LabelMarker("show", img_res, labels[i], colors[i])
        img_res = marker.start()
    cv2.imwrite(save_path, img_res)
    print(cs.blue(__name__), f": 已保存绘制结果到 {save_path}")


if __name__ == '__main__':
    mainMarkRect()









