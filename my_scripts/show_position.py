# -*- coding: utf-8 -*-
"""
Created on Wed Aug 18 10:10:31 2021

@author: 00769901
"""
import cv2


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
        

if __name__ == '__main__':
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
        
    # cv2.namedWindow("image", cv2.WINDOW_NORMAL)
    # cv2.setMouseCallback("image", on_EVENT_LBUTTONDOWN)
    # cv2.imshow("image", img)
    # cv2.waitKey(0)
    # print(a[0], b[0])









