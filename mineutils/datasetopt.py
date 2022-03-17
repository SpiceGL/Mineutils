import albumentations as album
import cv2
import numpy as np

from mineutils.path import Path
from mineutils.colorstr import ColorStr
from mineutils.fileio import Yaml


class DataAug:
    """
    --数据增强的一些处理方法。
    --img应为cv2类型图像矩阵，bbox应为(-1, 4)的numpy矩阵或为空, 且为yolo类型标注。
    --函数名带有单词img的，只处理图像不处理标注框；带有单词bbox的只处理标注框不处理图像；其他的同时处理图像和标注框。
    """
    @classmethod
    def randomFlipW(cls, img, bbox_yolo=None, rate=0.5):
        """
        --水平翻转。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox_yolo
        else:
            img = cv2.flip(img, 1)
            if bbox_yolo:
                bbox_yolo[:, 0] = 1 - bbox_yolo[:, 0]
            return img, bbox_yolo
    
    @classmethod
    def randomFlipH(cls, img, bbox_yolo=None, rate=0.5):
        """
        --垂直翻转。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox_yolo
        else:
            img = cv2.flip(img, 0)
            if bbox_yolo is None:
                bbox_yolo[:, 1] = 1 - bbox_yolo[:, 1]
            return img, bbox_yolo
        
    @classmethod
    def randomCropImgH(cls, img, bbox=None, rate=0.5, crop_range=(0, 0.5)):
        """
        --对输入图像从上往下裁或从下往上裁。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox
        else:
            ###一半几率从上往下裁，一半几率从下往上裁
            h, w = img.shape[0:2]
            crop_rate = cls._rand(*crop_range)
            if cls._rand(0, 1) < 0.5:
                t = round(h * crop_rate)
                img = img[t:h, :]
            else:
                b = round(h * (1 - crop_rate))
                img = img[0:b, :]
            return img, bbox

    @classmethod
    def randomCropImgW(cls, img, bbox=None, rate=0.5, crop_range=(0, 0.5)):
        """
        --对输入图像左往右裁或从右往左裁。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox
        else:
            ###一半几率从左往右裁，一半几率从右往左裁
            h, w = img.shape[0:2]
            crop_rate = cls._rand(*crop_range)
            if cls._rand(0, 1) < 0.5:
                l = round(w * crop_rate)
                img = img[:, l:w]
            else:
                b = round(w * (1 - crop_rate))
                img = img[:, 0:b]
            return img, bbox
        
    @classmethod
    def randomCropBboxH(cls, img, bbox_yolo, rate=0.5, crop_range=(0, 0.5)):
        """
        --对图像的标注框从上往下裁或从下往上裁。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox_yolo
        else:
            ###一半几率从上往下裁，一半几率从下往上裁
            x, y, w, h = np.split(bbox_yolo, 4, axis=-1)
            crop_rate = cls._rand(*crop_range) * h
            if cls._rand(0, 1) < 0.5:
                y = y + crop_rate / 2
                h = h - crop_rate
            else:
                y = y - crop_rate / 2
                h = h - crop_rate
            bbox_yolo = np.hstack((x, y, w, h))
            return img, bbox_yolo
    
    @classmethod
    def randomCropBboxW(cls, img, bbox_yolo, rate=0.5, crop_range=(0, 0.5)):
        """
        --对图像的标注框从左往右裁或从右往左裁。
        """
        if cls._rand(0, 1) > rate:
            return img, bbox_yolo
        else:
            ###一半几率从左往右裁，一半几率从右往左裁
            x, y, w, h = np.split(bbox_yolo, 4, axis=-1)
            crop_rate = cls._rand(*crop_range) * w
            if cls._rand(0, 1) < 0.5:
                x = x + crop_rate / 2
                w = w - crop_rate
            else:
                x = x - crop_rate / 2
                w = w - crop_rate
            bbox_yolo = np.hstack((x, y, w, h))
            return img, bbox_yolo
            
    
    @classmethod
    def randomAlbumImg(cls, img, bbox=None, jitter=0.3):
        """
        --数据增强库albumentations的一些只影响图像色彩不影响图像形状的操作。
        """
        trans = album.Compose(
            [
                album.RandomBrightness(limit=(-0.2, 0.5), p=0.8),
                album.OneOf([
                    album.IAAAdditiveGaussianNoise(),
                    album.GaussNoise(),
                ], p=0.3),
                album.OneOf([
                    album.MotionBlur(p=0.2),
                    album.MedianBlur(blur_limit=3, p=0.1),
                    album.Blur(blur_limit=3, p=0.1),
                ], p=0.5),

                album.OneOf([
                    album.CLAHE(clip_limit=2),
                    album.IAASharpen(),
                    album.IAAEmboss(),
                ], p=0.5),
                album.HueSaturationValue(p=0.3),
                album.JpegCompression(quality_lower=50, quality_upper=100, p=0.8)
            ])
        img = trans(image=img)["image"]
        return img, bbox
    
    @classmethod
    def _rand(cls, a=0., b=1.):
        return np.random.rand() * (b - a) + a


class YOLODataDiv:
    """
    --对YOLO类型的数据集进行划分, 并排除没有标签的图像。
    """
    @classmethod
    def div(cls, data_path: str, img_dirname="imgs", label_dirname="labels",
            img_suffix=".jpg", ignore_names=("classes.txt",), shuffle=False, rate=(1.0, 0, 0, 0)):
        """
        --数据集划分。
        """
        assert 0 <= sum(rate) <= 1, "请正确划分数据集比例！"
        print(ColorStr.blue("函数YOLODataDiv.div():"), "尽量避免使分割后的最小非空数据集大小小于10。")
        save_names = ["train.yaml", "val.yaml", "trainval.yaml", "test.yaml"]
        img_paths, label_paths = cls._getAllDatapaths(data_path, img_dirname, label_dirname,
                                                      img_suffix, ignore_names, shuffle)
        
        data_len = len(img_paths)
        ###确定划分后的分段位置，
        div_idx = [0]
        for i in range(len(rate)):
            div_idx.append(div_idx[-1] + round(rate[i] * data_len))
        if sum(rate) == 1:
            div_idx[-1] = data_len

        for i in range(len(rate)):
            if rate[i]:
                save_dict = {"img_paths": img_paths[div_idx[i]: div_idx[i + 1]],
                             "label_paths": label_paths[div_idx[i]: div_idx[i + 1]]}
                save_path = Path.join(data_path, save_names[i])
                Yaml.save(save_path, save_dict)
            
    @classmethod
    def _getAllDatapaths(cls, data_path: str, img_dirname: str, label_dirname: str,
                         img_suffix: str, ignore_names: list, shuffle: bool):
        """
        --读取数据集里所有有标签的数据。
        """
        img_dirpath = Path.join(data_path, img_dirname)
        label_dirpath = Path.join(data_path, label_dirname)
        label_paths = Path.listDir(label_dirpath, ignore_names=ignore_names)
        img_paths = [Path.join(img_dirpath, Path.splitName(lp, suffix=False) + img_suffix) for lp in label_paths]
        assert len(img_paths) == len(label_paths), "图像数量与标签数量不相等！"
        if shuffle:
            state = np.random.get_state()
            np.random.shuffle(img_paths)
            np.random.set_state(state)
            np.random.shuffle(label_paths)
        return img_paths, label_paths


class BboxOpt:
    @classmethod
    def constructBbox(cls, points, return_ltrb=False, use_round=False):
        """
        --通过四个手动标记的点生成一个矩形选择框，xy按CV2顺序排列。
        --四个点标记应互不重合。
        """
        point_lt, point_lb, point_rt, point_rb = None, None, None, None
        point1, point2, point3, point4 = points
        center = [sum([point1[0], point2[0], point3[0], point4[0]]) / 4,
                  sum([point1[1], point2[1], point3[1], point4[1]]) / 4]
        x0, y0 = center  ###中心点
        for i, point in enumerate(points):
            if point[0] < x0 and point[1] < y0:
                point_lt = point
                points.pop(i)
                break
        for i, point in enumerate(points):
            if point[0] < x0 and point[1] > y0:
                point_lb = point
                points.pop(i)
                break
        for i, point in enumerate(points):
            if point[0] > x0 and point[1] < y0:
                point_rt = point
                points.pop(i)
                break
        for i, point in enumerate(points):
            if point[0] > x0 and point[1] > y0:
                point_rb = point
                points.pop(i)
                break
        assert point_lt and point_lb and point_rt and point_rb
        width = (point_rt[0] + point_rb[0] - point_lt[0] - point_lb[0]) / 2
        height = (point_lb[1] + point_rb[1] - point_lt[1] - point_rt[1]) / 2
        if return_ltrb:
            x1, y1, x2, y2 = cls.xywh2ltrb((x0, y0, width, height), use_round)
            return x1, y1, x2, y2
        else:
            if use_round:
                x0, y0, width, height = cls.bboxRound((x0, y0, width, height))
            return x0, y0, width, height
    
    @classmethod
    def xywh2ltrb(cls, bbox, use_round=False):
        x, y, w, h = bbox
        
        left = x - w / 2
        right = x + w / 2
        top = y - h / 2
        bottom = y + h / 2
        if use_round:
            left, top, right, bottom = cls.bboxRound((left, top, right, bottom))
        return left, top, right, bottom
    
    @classmethod
    def ltrb2xywh(cls, bbox, use_round=False):
        l, t, r, b = bbox
        w = r - l
        h = b - t
        x = l + w / 2
        y = t + h / 2
        if use_round:
            x, y, w, h = cls.bboxRound((x, y, w, h))
        return x, y, w, h
    
    @classmethod
    def bboxRound(cls, bbox):
        a, b, c, d = bbox
        a, b, c, d = round(a), round(b), round(c), round(d)
        return a, b, c, d
        

def normalize(img, mean=(0.485, 0.456, 0.406), std=(0.229, 0.224, 0.225), max_pixel_value=1.0):
    """
    --图像标准化，输入图片应是RGB格式。
    """
    mean = np.array(mean, dtype=np.float32)
    mean *= max_pixel_value

    std = np.array(std, dtype=np.float32)
    std *= max_pixel_value

    denominator = np.reciprocal(std, dtype=np.float32)

    img = img.astype(np.float32)
    img -= mean
    img *= denominator
    return img


if __name__ == "__main__":
    import cv2
    
    img = np.random.randn(224, 224, 3)
    img = cv2.resize(img, (112, 224))
    img = img * 255
    cv2.imshow("test1", img)
    
    cv2.waitKey(0)


