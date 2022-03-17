# -*- coding: utf-8 -*-
"""
Created on Thu Aug 19 11:09:57 2021

@author: 00769901
"""

import os
import cv2
import sys
sys.path.append("..")
from mineutils.path import Path
from mineutils.quickshow import setWindowCV, loopShowCV


def videoToPics(video_path, save_dir, show_frame=False, save_interval=0):
    Path.makedirs(save_dir)
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
            loopShowCV(window_name, frame, wait=1, user_break=True)
        save_name = video_name + "_" + str(current).zfill(5) + ".jpg"
        if save_interval:
            if current % save_interval == 0:
                cv2.imwrite(os.path.join(save_dir, save_name), frame)
        else:
            cv2.imwrite(os.path.join(save_dir, save_name), frame)
    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    cv2.destroyAllWindows()
    video_path = "../Videos/1027/change_angle/Normal_Sat_Oct_23_16_06_00_2021.avi"
    save_dir = "../Datas/change_angle"
    videoToPics(video_path, save_dir, show_frame=True, save_interval=20)
