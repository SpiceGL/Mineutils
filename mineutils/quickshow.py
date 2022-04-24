# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 15:04:28 2021

@author: 00769901
"""
import cv2

from mineutils import Error
from mineutils.colorstr import ColorStr


def quickShowCV(window_name: str,
                img: "ndarray",
                flag=cv2.WINDOW_NORMAL, 
                size=(),
                position=(),
                wait=0, 
                close=False):
    """
    --快速进行一次显示CV2图像的操作。
    """
    cv2.namedWindow(window_name, flag)
    if size:
        cv2.resizeWindow(window_name, *size)
    if position:
        cv2.moveWindow(window_name, *position)
        
    cv2.imshow(window_name, img)
    k = cv2.waitKey(wait)
    if close:
        cv2.destroyWindow(window_name)
    


def setWindowCV(window_name: str,
                flag=cv2.WINDOW_NORMAL,
                size=(),
                position=()) -> str:
    """
    --设置CV2窗口属性，返回窗口名。
    """
    cv2.namedWindow(window_name, flag)
    if size:
        cv2.resizeWindow(window_name, *size)
    if position:
        cv2.moveWindow(window_name, *position)

    return window_name


def loopShowCV(window_name: str,
               img,
               wait=0,
               user_break=False):
    """
    --用于在循环中显示CV2图像。
    --与setWindowCV配合使用，避免循环中重复设置window。
    """
    cv2.imshow(window_name, img)
    k = cv2.waitKey(wait)
    if user_break:
        if k & 0xff == 27 or k & 0xff == ord("q"):
            cv2.destroyAllWindows()
            raise Error("函数loopShowCV: 用户主动停止循环并退出程序")
    return k


if __name__ == "__main__":
    img_path = "../Datasets/Z75_Z86_ZTC950V763_20211112/imgs/Z75_img00001.jpg"
    img = cv2.imread(img_path)
    quickShowCV("111", img, 0, size=(640, 480))

    
    

