import numpy as np
from paddle.io import Dataset

from mineutils.path import Path
from mineutils.datasetopt import DataAug, normalize
from mineutils.fileio import Yaml
from mineutils.randomfix import identicalShuffle
import cv2


class ObjDetDataset(Dataset):
    """
    --用于读取目标检测类型的数据集。
    --读取前因先用dataoption.YOLODataDiv划分数据集。
    """
    def __init__(self, data_path: str, resize_size=(0, 0), data_norm=False, mode="val", shuffle=False, use_dataaug=False, load_rate_when_shuffle=None):
        super(ObjDetDataset, self).__init__()
        self.data_path = data_path
        self.resize_size = resize_size
        self.data_norm = data_norm
        self.mode = mode
        self.use_dataaug = use_dataaug
        self.img_paths, self.label_paths = self._getDatapaths(data_path, shuffle, load_rate_when_shuffle)

    def _getDatapaths(self, data_path, shuffle, load_rate_when_shuffle):
        """
        --获取划分好的数据路径。
        """
        loading_path = Path.join(data_path, self.mode+".yaml")
        loading_dict = Yaml.load(loading_path)
        img_paths = loading_dict["img_paths"]
        label_paths = loading_dict["label_paths"]
        assert len(img_paths) == len(label_paths), "读取到的图像数量与标签数量不相等！"
        if shuffle:
            identicalShuffle(img_paths, label_paths)
            ###数据集太大epoch训练慢时，可以一个epoch只加载一定比例的数据
            if load_rate_when_shuffle:
                load_num = int(len(img_paths) * load_rate_when_shuffle)
                img_paths = img_paths[0:load_num]
                label_paths = label_paths[0:load_num]
            
        return img_paths, label_paths
    
    def _readLabels(self, label_path: str, label_type="txt"):
        """
        --读取以txt格式保存的标注框信息。
        """
        label = []
        bbox = []
        with open(label_path, "rb") as f:
            lines = f.readlines()
            for line in lines:
                l, x, y, w, h = line.split()
                l = int(l)
                x = float(x)
                y = float(y)
                w = float(w)
                h = float(h)
                label.append(l)
                bbox.append([x, y, w, h])
        label = np.array(label).reshape(-1)
        bbox = np.array(bbox).reshape(-1, 4)
        return label, bbox
    
    def _dataAug(self, img, bbox):
        """
        --对数据集进行增强处理。
        """
        img, bbox = DataAug.randomFlipW(img, bbox)
        return img, bbox

    def __len__(self):
        return len(self.img_paths)

    def __getitem__(self, idx):
        w_in, h_in = self.resize_size
        img = cv2.imread(self.img_paths[idx])
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.resize(img, (w_in, h_in))
        img = normalize(img)
        label, bbox = self._readLabels(self.label_paths[idx])
        if self.use_dataaug:
            img, bbox = self._dataAug(img, bbox)
        img = np.transpose(img, (2, 0, 1))
        return img/255., label, bbox
    
    
    

if __name__ == "__main__":
    from mineutils import useProjDir
    from paddle.io import DataLoader
    useProjDir()
    data_path = "Datasets/Z75_Z86_ZTC950V763_20211112"
    
    dataset = ObjDetDataset(data_path, (224, 448), "train")
    loader = DataLoader(dataset, batch_size=2, shuffle=True, drop_last=True, num_workers=0)
    for i, l, p in loader():
        print(i)
        print(l)
        print(p)
        break
