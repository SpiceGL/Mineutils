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

random.seed(7)


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
                print(cs.green("BoxMarker:"), "矩形框已完成，如需修改继续点击修改最近点，或按'Backspace'删除所有点。如确认无误请按'Enter'键继续！")
    
    def start(self, ltrb=None, kill_window=False, open_recall=False):
        cv2.namedWindow(self.title, cv2.WINDOW_AUTOSIZE)
        # cv2.resize(self.title, self.window_size[0], self.window_size[1])
        cv2.moveWindow(self.title, self.position[0], self.position[1])
        cv2.setMouseCallback(self.title, self._onclick)
        if ltrb is not None:
            self.points = []
            self.lines = []
            self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[0], ltrb[1], None, None)
            self._onclick(cv2.EVENT_LBUTTONDOWN, ltrb[2], ltrb[3], None, None)
        else:
            cv2.imshow(self.title, self.img)
        k_esc = 27
        k_delete = 8
        k_enter = 13
        k_space = 32
        k_next = ord('d')
        k_prior = ord('a')   #撤回
        while True:
            k = cv2.waitKey(0)
            if k & 0xff == k_esc:
                cv2.destroyAllWindows()
                raise Error("BoxMarker.start(): 用户主动中止！")
            if k & 0xff == k_delete:
                self.points = []
                self.lines = []
                self._drawMark()
            if (k & 0xff == k_enter) or (k & 0xff == k_space) or (k & 0xff == k_next):
                if len(self.points) < 2:
                    print(cs.yellow("BoxMarker:"), "矩形框未设置完成，请继续绘制对角点！")
                else:
                    break
            if open_recall and (k & 0xff == k_prior):
                if len(self.points) == 2:
                    return self.points, True
                else:
                    return [[None, None], [None, None]], True
        if kill_window:
            cv2.destroyWindow(self.title)
        return self.points, False


def random_cutsize(max_size):
    res = 2 * random.random() - 1
    res *= max_size
    return res


def mainWindingImg():
    classes = ["normal", "wrong"]
    videos_dirpath = r"E:\Datasets\Z75\Normal"
    label = 0

    save_interval = 10   #>=1
    
    rand_cut_rate = 0.1
    
    imgsave_dirpath = Path.join(videos_dirpath, "imgs")
    labelsave_dirpath = Path.join(videos_dirpath, "labels")
    video_paths = Path.listDir(videos_dirpath)
    
    if not Path.exists(imgsave_dirpath):
        Path.makeDirs(imgsave_dirpath)
    if not Path.exists(labelsave_dirpath):
        Path.makeDirs(labelsave_dirpath)
        
    with open(Path.join(labelsave_dirpath, "classes.txt"), "wb") as f:
        for ctx in classes:
            f.write(ctx.encode("UTF-8"))
            f.write("\n".encode("UTF-8"))

    
    for vpath in video_paths:
        cap = cv2.VideoCapture()
        cap.open(vpath)
        if not cap.isOpened():
            print(f"打开{vpath}失败, 已跳过！")
            continue
        frame_id = 0
        pre_frame_id = 0   #上一次循环中的frame_id
        label_dict = {}
        while cap.isOpened():
            savename = Path.splitName(vpath, False) + "_" + str(frame_id).zfill(5)
            imgsave_path = Path.join(imgsave_dirpath, savename + ".jpg")
            labelsave_path = Path.join(labelsave_dirpath, savename + ".txt")
            if Path.exists(imgsave_path):
                Path.remove(imgsave_path)
                print(cs.fuchsia("mainWindingImg:"), f"已删除{imgsave_path}")
            if Path.exists(labelsave_path):
                Path.remove(labelsave_path)
                print(cs.fuchsia("mainWindingImg:"), f"已删除{labelsave_path}")
            
            cap.set(cv2.CAP_PROP_POS_FRAMES, frame_id)
            ret, frame = cap.read()
            if not ret:
                print(cs.blue("mainWindingImg:"), "视频结束。")
                cv2.destroyAllWindows()
                break
            if frame_id == 0:
                label_dict.clear()
                cv2.destroyAllWindows()
                print(cs.blue("mainWindingImg:"), "当前是视频首帧，请选取裁剪框")
                cutbox_marker = BoxMarker(vpath, frame, (720, 480), (600, 0))
                [cut_pt0, cut_pt1], _ = cutbox_marker.start(kill_window=True, open_recall=False)
                
                cut_l = min(cut_pt0[0], cut_pt1[0])
                cut_t = min(cut_pt0[1], cut_pt1[1])
                cut_r = max(cut_pt0[0], cut_pt1[0])
                cut_b = max(cut_pt0[1], cut_pt1[1])

            print(cs.blue("mainWindingImg:"), f"当前操作的是视频第{frame_id}帧，请标注label")
            cut_frame = frame[cut_t:cut_b, cut_l:cut_r]
            labelbox_marker = BoxMarker("label", cut_frame, (720, 240), (600, 150))
            if frame_id in label_dict:
                [label_incut_pt0, label_incut_pt1], recall = labelbox_marker.start(label_dict[frame_id], kill_window=False, open_recall=True)
            elif frame_id - save_interval in label_dict:
                [label_incut_pt0, label_incut_pt1], recall = labelbox_marker.start(label_dict[frame_id - save_interval], kill_window=False, open_recall=True)
            else:
                [label_incut_pt0, label_incut_pt1], recall = labelbox_marker.start(kill_window=False, open_recall=True)
            label_dict[frame_id] = [label_incut_pt0[0], label_incut_pt0[1], label_incut_pt1[0], label_incut_pt1[1]]
            
            if recall:
                label_dict.pop(frame_id)
                frame_id = max(0, frame_id - save_interval)
                continue
            # label框在原图上的对应坐标
            label_l = min(label_incut_pt0[0], label_incut_pt1[0]) + cut_l
            label_t = min(label_incut_pt0[1], label_incut_pt1[1]) + cut_t
            label_r = max(label_incut_pt0[0], label_incut_pt1[0]) + cut_l
            label_b = max(label_incut_pt0[1], label_incut_pt1[1]) + cut_t
            
            max_cut_size = rand_cut_rate * abs(cut_b - cut_t)   #最大裁剪量
            
            final_cut_l = round(max(0, cut_l + random_cutsize(max_cut_size)))
            final_cut_t = round(max(0, cut_t + random_cutsize(max_cut_size)))
            final_cut_r = round(min(frame.shape[1], cut_r + random_cutsize(max_cut_size)))
            final_cut_b = round(min(frame.shape[0], cut_b + random_cutsize(max_cut_size)))
            final_cut_w = final_cut_r - final_cut_l
            final_cut_h = final_cut_b - final_cut_t
            
            final_label_x = max(0, label_l - final_cut_l)
            final_label_y = max(0, label_t - final_cut_t)
            final_label_w = min(label_r - final_cut_l, final_cut_r - final_cut_l) - final_label_x   #label在裁剪图上的右坐标和裁剪图最大宽度中较小的一个，再减去label在裁剪图上的左坐标
            final_label_h = min(label_b - final_cut_t, final_cut_b - final_cut_t) - final_label_y
            final_label_cx = final_label_x + final_label_w / 2   #中心点x
            final_label_cy = final_label_y + final_label_h / 2   #中心点x
            
            cv2.imwrite(imgsave_path, frame[final_cut_t:final_cut_b, final_cut_l:final_cut_r])
            print(cs.green("mainWindingImg:"), f"已保存{imgsave_path}")
            with open(labelsave_path, "wb") as f:
                ctx = f"{label} {final_label_cx / final_cut_w} {final_label_cy / final_cut_h} {final_label_w / final_cut_w} {final_label_h / final_cut_h}"
                f.write(ctx.encode("UTF-8"))
            print(cs.green("mainWindingImg:"), f"已保存{labelsave_path}")
            
            frame_id += save_interval
            print("--------------------------------------------------------------")
            

if __name__ == '__main__':
    mainWindingImg()


