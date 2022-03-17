from matplotlib import pyplot as plt
import os
import cv2
import numpy as np

from mineutils.colorstr import ColorStr
from mineutils import Error
from mineutils.fileio import Yaml, Pkl
from mineutils.path import Path


class History:
    """
    --在训练时保存训练与验证的loss值，并绘图。
    """
    def __init__(self, save_dir="./", save_name="History", modes=("train", "val")):
        self.save_dir = save_dir
        self.save_name = save_name
        self.modes = modes
        
        self.losses = {mode: [] for mode in modes}
        self.acces = {mode: [] for mode in modes}
        self.best_loss = {mode: 1e10 for mode in modes}
        self.best_acc = {mode: 0 for mode in modes}
        self.is_bestloss = {mode: False for mode in modes}
        self.is_bestacc = {mode: False for mode in modes}
    
    def appendLoss(self, loss, mode="train"):
        loss = float(loss)
        if mode in self.modes:
            self.losses[mode].append(loss)
            self._updateBest(loss, "loss", mode)
        else:
            raise Error("trainHistory.appendLoss(): 'mode'的值填写错误！")
        
    def appendAcc(self, acc, mode="train"):
        acc = float(acc)
        if mode in self.modes:
            self.acces[mode].append(acc)
            self._updateBest(acc, "acc", mode)
        else:
            raise Error("trainHistory.appendAcc(): 'mode'的值填写错误！")
        
    def _updateBest(self, value, tag: str, mode: str):
        if tag == "loss":
            if value < self.best_loss[mode]:
                self.best_loss[mode] = value
                self.is_bestloss[mode] = True
            else:
                self.is_bestloss[mode] = False
        elif tag == "acc":
            if value > self.best_acc[mode]:
                self.best_acc[mode] = value
                self.is_bestacc[mode] = True
            else:
                self.is_bestacc[mode] = False
        else:
            raise Error("History._updateBest(): tag输入有误！")
    
    def plot(self):
        """
        ax.legend(): loc参数:           ax.plot(): color参数:
            'best'           0              'r'    'red'
            'upper right'    1              'g'    'green'
            'upper left'     2              'b'    'blue'
            'lower left'     3              'c'    'cyan'
            'lower right'    4              'm'    'magenta'
            'right'          5              'y'    'yellow'
                                            'w'    'white'
             ......                          ......
        """
        iters = range(len(self.losses["train"]))
        
        fig, ax1 = plt.subplots()
        ax1.plot(iters, self.losses["train"], 'red', linestyle='-', linewidth=2, label='train loss')
        if self.losses["val"]:
            ax1.plot(iters, self.losses["val"], 'coral', linestyle='--', linewidth=2, label='val loss')
        ax1.set_xlabel("Epoch")
        ax1.set_ylabel("Loss")
        ax1.legend(loc=3)
        if self.acces["train"]:
            ax2 = ax1.twinx()
            ax2.plot(iters, self.acces["train"], 'blue', linestyle='-', linewidth=2, label='train acc')
            if self.acces["val"]:
                ax2.plot(iters, self.acces["val"], 'cyan', linestyle='--', linewidth=2, label='val acc')
            ax2.set_ylabel("Acc")
            ax2.legend(loc=2)
        plt.grid(True)
        
        plt.savefig(os.path.join(self.save_dir, self.save_name + ".png"))
        
    def save(self):
        save_dict = {"loss": self.losses, "acc": self.acces,
                     "best_loss": self.best_loss, "best_acc": self.best_acc}
        Yaml.save(Path.join(self.save_dir, self.save_name + ".yaml"), save_dict)
        
    def load(self, path):
        history_dict = Yaml.load(path)
        self.losses = history_dict["loss"]
        self.acces = history_dict["acc"]
        self.best_loss = history_dict["best_loss"]
        self.best_acc = history_dict["best_acc"]


class PixelMarker:
    def __init__(self, title, img, window_size=(720, 480), position=(0, 0), norm=False):
        self.title = title
        self.img = img
        self.size = (img.shape[1], img.shape[0])
        self.window_size = window_size
        self.position = position
        self.points = []
        self.norm = norm
    
    def _drawMark(self):
        img_drawn = self.img.copy()
        for xy in self.points:
            x, y = xy[0], xy[1]
            cv2.circle(img_drawn, (x, y), 3, (0, 0, 255), thickness=-1)
            text_x, text_y = self._setTextPosition((x, y))
            cv2.putText(img_drawn, str(xy), (text_x, text_y), cv2.FONT_HERSHEY_DUPLEX, 0.5, (255, 255, 255),
                        thickness=1)
        cv2.imshow(self.title, img_drawn)
        
    def _setTextPosition(self, point):
        x, y = point
        w, h = self.size
        if x > w / 5 * 3:
            x = x - w // 10
        if y < h / 10:
            y = y + h // 10
        return x, y
    
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
                raise Error("PixelMarker.start(): 用户主动中止！")
            if k & 0xff == ord("d"):
                self.points.pop()
                self._drawMark()
            if k & 0xff == 13:
                break
        cv2.destroyWindow(self.title)
        points = self.points
        if self.norm:
            points = []
            for xy in self.points:
                x = xy[0] / self.size[0]
                y = xy[1] / self.size[1]
                points.append((x, y))
        print(ColorStr.blue("PixelMarker.start()返回的点:"), points)
        return points


def putLabel(img, label, text_thickness=None, label_position=None, bg_color=None):
    tt = text_thickness or round(0.002 * (img.shape[0] + img.shape[1]) / 2) + 1  ###标签显示大小
    c0 = label_position or [img.shape[1] // 2, img.shape[0] // 2]  ###预设文字显示位置中心点
    bg_color = bg_color or [np.random.randint(0, 255) for _ in range(3)]  ###文字显示的背景颜色
    tf = max(tt - 1, 1)   ###
    t_size = cv2.getTextSize(label, 0, fontScale=tt / 3, thickness=tf)[0]
    c1 = (c0[0] - t_size[0] // 2, c0[1] + t_size[1] // 2)
    c2 = (c1[0] + t_size[0], c1[1] - t_size[1])
    cv2.rectangle(img, c1, c2, bg_color, thickness=-1, lineType=cv2.LINE_AA)  ###绘制label背景
    cv2.putText(img, label, (c1[0], c1[1]), 0, tt / 3, [225, 255, 255], thickness=tf, lineType=cv2.LINE_AA)
    return img


def putBbox(img, bbox_ltrb, label=None, bbox_thickness=None, bbox_color=None):
    """
    --输入图像和需要绘制的框的两个角点坐标，绘制图像。
    """
    x1, y1, x2, y2 = bbox_ltrb
    c1, c2 = (x1, y1), (x2, y2)
    bt = bbox_thickness or round(0.002 * (img.shape[0] + img.shape[1]) / 2) + 1  ###标签显示大小
    bbox_color = bbox_color or [np.random.randint(0, 255) for _ in range(3)]  ###bbox的显示颜色
    cv2.rectangle(img, c1, c2, bbox_color, thickness=bt, lineType=cv2.LINE_AA)
    if label:
        tf = max(bt - 1, 1)  ###
        t_size = cv2.getTextSize(label, 0, fontScale=bt / 3, thickness=tf)[0]
        c2 = (c1[0] + t_size[0], c1[1] - t_size[1])
        cv2.rectangle(img, c1, c2, bbox_color, thickness=-1, lineType=cv2.LINE_AA)  ###绘制label背景
        cv2.putText(img, label, (c1[0], c1[1] - 2), 0, bt / 3, [225, 255, 255], thickness=tf, lineType=cv2.LINE_AA)
    return img
    


if __name__ == "__main__":
    from mineutils import useProjDir
    import numpy as np
    useProjDir()
    his_path = "Models/021/History.pkl"
    H = History()
    H.appendAcc(0.08)
    H.appendAcc(1.5)



