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
                flag=cv2.WINDOW_AUTOSIZE,
                size=(),
                position=(),
                wait=1,
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
    k = cv2.waitKey(wait) & 0xff
    if close:
        cv2.destroyWindow(window_name)
    


def setWindowCV(window_name: str,
                flag=cv2.WINDOW_AUTOSIZE,
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
               wait=1):
    """
    --用于在循环中显示CV2图像。
    --与setWindowCV配合使用，避免循环中重复设置window。
    """
    cv2.imshow(window_name, img)
    k = cv2.waitKey(wait) & 0xff
    return k
    # if k == 27 or k == ord('q'):
    #     return False
    # else:
    #     return True


if __name__ == "__main__":
    img_path = "../Datasets/Z75_Z86_ZTC950V763_20211112/imgs/Z75_img00001.jpg"
    img = cv2.imread(img_path)
    quickShowCV("111", img, 0, size=(640, 480))

    
    

