import cv2
import os
from collections import OrderedDict
from mineutils.colorstr import ColorStr as cs
from mineutils.quickshow import quickShowCV, setWindowCV, loopShowCV
from mineutils.path import Path


def distribute(data_dir, rule_od):
    file_paths = Path.listDir(data_dir)
    setWindowCV("show")
    
    for fp in file_paths:
        img = cv2.imread(fp)
        k = loopShowCV("show", img)
        # if k & 0xff == ord(""):


def main():
    data_dir = "E:/Project/distributing/normal_0509"
    data_name = Path.splitName(data_dir)
    key_name_dict = OrderedDict({"most": "normal",
                                 "n": "normal",
                                 "h": "hard_normal",
                                 "b": "bad_normal",
                                 "a": "abnormal"})
    start_idx = 5
    stop = "0"
    stop_key = "0"
    quick_key = "+"
    most, n, h, b, a = key_name_dict.keys()
    dir_n = Path.join(Path.parent(data_dir), key_name_dict[n])
    dir_h = Path.join(Path.parent(data_dir), key_name_dict[h])
    dir_b = Path.join(Path.parent(data_dir), key_name_dict[b])
    dir_a = Path.join(Path.parent(data_dir), key_name_dict[a])
    dir_most = Path.join(Path.parent(data_dir), key_name_dict[most])
    
    Path.makeDirs(dir_n)
    Path.makeDirs(dir_h)
    Path.makeDirs(dir_b)
    Path.makeDirs(dir_a)
    
    file_names = Path.listImg(data_dir, return_path=False)
    setWindowCV("show", size=(720, 480))
    idx = start_idx
    stop = False
    while idx < len(file_names):
        if idx < 0:
            idx = 0
        fn = file_names[idx]
        fp = Path.join(data_dir, fn)
        print(cs.green("reading:"), fp)
        img = cv2.imread(fp)
        again = True
        while again:
            if stop:
                k = loopShowCV("show", img, 0)
            else:
                k = loopShowCV("show", img, 200)
            if k & 0xff == ord(n):
                Path.copy(fp, dir_n)
                Path.rename(Path.join(dir_n, fn), Path.join(dir_n, data_name + "_" + fn))
                again = False
                idx += 1
            elif k & 0xff == ord(h):
                Path.copy(fp, dir_h)
                Path.rename(Path.join(dir_h, fn), Path.join(dir_h, data_name + "_" + fn))
                again = False
                idx += 1
            elif k & 0xff == ord(b):
                Path.copy(fp, dir_b)
                Path.rename(Path.join(dir_b, fn), Path.join(dir_b, data_name + "_" + fn))
                again = False
                idx += 1
            elif k & 0xff == ord(a):
                Path.copy(fp, dir_a)
                Path.rename(Path.join(dir_a, fn), Path.join(dir_a, data_name + "_" + fn))
                again = False
                idx += 1
            elif k == -1:   ###无输入时
                Path.copy(fp, dir_most)
                Path.rename(Path.join(dir_most, fn), Path.join(dir_most, data_name + "_" + fn))
                again = False
                idx += 1
            elif k & 0xff == ord("4"):
                idx -= 1
                if idx < 0:
                    idx = 0
                fn = file_names[idx]
                fp = Path.join(data_dir, fn)
                x = Path.join(dir_n, data_name + "_" + fn)
                if Path.exists(Path.join(dir_n, data_name + "_" + fn)):
                    Path.remove(Path.join(dir_n, data_name + "_" + fn))
                    print(cs.blue("remove image:"), Path.join(dir_n, data_name + "_" + fn))
                elif Path.exists(Path.join(dir_h, data_name + "_" + fn)):
                    Path.remove(Path.join(dir_h, data_name + "_" + fn))
                    print(cs.blue("remove image:"), Path.join(dir_h, data_name + "_" + fn))
                elif Path.exists(Path.join(dir_b, data_name + "_" + fn)):
                    Path.remove(Path.join(dir_b, data_name + "_" + fn))
                    print(cs.blue("remove image:"), Path.join(dir_b, data_name + "_" + fn))
                elif Path.exists(Path.join(dir_a, data_name + "_" + fn)):
                    Path.remove(Path.join(dir_a, data_name + "_" + fn))
                    print(cs.blue("remove image:"), Path.join(dir_a, data_name + "_" + fn))
                else:
                    print(cs.yellow("warning:"), "没有找到上一张被分发的图像，可能已退回到第一张！")
                stop = True
                again = False
            elif k & 0xff == ord(stop_key):
                stop = True
                again = True
            elif k & 0xff == ord(quick_key):
                stop = False
                again = True
            else:
                stop = True
                again = True
                print(cs.yellow("选择文件夹错误，请重新输入！"), k)
            
            
            
                
    
            

if __name__ == '__main__':
    main()
    # file_path = '2/'
    # files = os.listdir(file_path)
    # s = "5"
    # s_dict = OrderedDict({"w": "0",
    #                       "s": "1",
    #                       "a": "2",
    #                       "d": "3"})
    # for i in range(0, len(files)):
    #     # message = "_next"
    #     # for j in range(3):
    #     #     ff = files[i+j]
    #     #     path = os.path.join(file_path, ff)
    #     #     img = cv2.imread(path)
    #     #     h, w, c = img.shape
    #     #     img = img.reshape((h, 4, w//4, c))
    #     #     img1 = img[:, 0]
    #     #     quickShowCV("now"+message*j, img1, 0, (525, 350), (1200, 350*j), 1)
    #     # path = os.path.join(file_path, files[i])
    #     ff = files[i]
    #     path = os.path.join(file_path, ff)
    #     img = cv2.imread(path)
    #     h, w, c = img.shape
    #     img = img.reshape((h, 4, w//4, c))
    #     img1 = img[:, 0]
    #     quickShowCV("now", img1, 0, (720, 480), (0, 0), 1)
    #     s_input = input(ColorStr.blue(f"第{i}张：Enter your input: "))
    #     if s_input:
    #         s = s_dict[s_input[0]]
    #     if int(s) < 5:
    #         new_path = path[:-4].split("-")[0]+"-" + s + path[-4:]
    #         os.rename(path, new_path)
    #         print("将图像"+ColorStr.fuchsia(ff)+"重命名为"+ColorStr.fuchsia(Path.splitName(new_path)))
    #     else:
    #         new_path = path[:-4].split("-")[0] + path[-4:]
    #         os.rename(path, new_path)
    #         print("将图像" + ColorStr.fuchsia(ff) + "重命名为" + ColorStr.fuchsia(Path.splitName(new_path)))
    #         # print("图像" + ColorStr.fuchsia(ff) + "未被重命名")

