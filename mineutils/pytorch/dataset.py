from torch.utils.data import Dataset
import os
import numpy as np
from mineutils.path import Path


class MultiDataset(Dataset):
    """
    --读取多个数据集并列组成的大数据集。
    --数据集的结构应为：
        1 大数据集
            1.1 数据集1
                1.1.1 imgs
                1.1.2 labels
            1.2 ……
    """
    def __init__(self, root_path: str, data_names: list, img_dirname="imgs", label_dirname="labels", ignore_imgs=[], ignore_labels=[], shuffle=True):
        super().
        self.root_path = root_path
        self.data_paths = [os.path.join(root_path, data_name) for data_name in data_names]
        self.img_paths, self.labels_paths = self._get_all_datapath(self.data_paths, img_dirname, label_dirname, ignore_imgs, ignore_labels, shuffle)
        
        
    def _get_all_datapath(self, data_paths: list, img_dirname: str, label_dirname: str,
                          ignore_imgs: list, ignore_labels: list, shuffle: bool) -> list:
        
        img_paths = []
        lable_paths = []
        for data_path in data_paths:
            img_dirpath = os.path.join(data_path, img_dirname)
            img_paths.extend(Path.listdir(img_dirpath, ignore_names=ignore_imgs))
            label_dirpath = os.path.join(data_path, label_dirname)
            lable_paths.extend(Path.listdir(label_dirpath, ignore_names=ignore_labels))
        if shuffle:
            state = np.random.get_state()
            np.random.shuffle(img_paths)
            np.random.set_state(state)
            np.random.shuffle(lable_paths)
    
    def __len__(self):
        pass
    
    def __getitem__(self, idx):
        pass
    
    
if __name__ == "__main__":
    img_paths = [1,2,3,4,5,6,7,8,9,10]
    lable_paths = [1,2,3,4,5,6,7,8,9,10]
    state = np.random.get_state()
    np.random.shuffle(img_paths)
    np.random.set_state(state)
    np.random.shuffle(lable_paths)
    
    print(img_paths)
    print(lable_paths)