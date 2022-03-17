import numpy as np
import os
import random


def setSeedBase(seed: int):
    """
    --固定python基础程序的随机种子。
    """
    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    np.random.seed(seed)


def setSeedTorch(seed: int, speed_up=True):
    """
    --固定pytorch程序的随机种子。
    --speed_up会带来少量随机性，但能提高运行效率。
    """
    import torch
    import torch.backends.cudnn as cudnn

    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = not speed_up   ###确定性固定
    torch.backends.cudnn.benchmark = speed_up   ###True性能更好
    # torch.backends.cudnn.enable = True
    

def workerInitFnTorch(worker_id):
    """
    --pytorch使用Dataloader多线程加载数据的时候，为每个线程设置单独的随机种子。
    """
    seed = 100
    seed += worker_id
    setSeedTorch(seed)
    
    
def setSeedPaddle(seed: int):
    import paddle
    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    np.random.seed(seed)
    paddle.seed(seed)
    

def workerInitFnPaddle(worker_id):
    """
    --paddle使用Dataloader多线程加载数据的时候，为每个线程设置单独的随机种子。
    --实测似乎没啥用处，
    """
    seed = 100
    seed += worker_id
    setSeedPaddle(seed)
    

def identicalShuffle(*lists):
    """
    --打乱顺序的过程中保持多个list的元素对应关系。
    """
    state = np.random.get_state()
    for listX in lists:
        np.random.set_state(state)
        np.random.shuffle(listX)
    return lists


if __name__ == "__main__":
    A = list(range(10))
    B = list(range(10))
    C = list(range(10))
    ABC = identicalShuffle(A, B, C)
    print(ABC)
