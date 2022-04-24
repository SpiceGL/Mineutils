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
    data_dir = "E:/Project/distributing/normal_0419"
    data_name = Path.splitName(data_dir)
    key_name_dict = OrderedDict({"n": "normal",
                                 "h": "hard_normal",
                                 "b": "bad_normal",
                                 "a": "abnormal"})
    a, b, c, d = key_name_dict.keys()
    dir_a = Path.join(Path.parent(data_dir), key_name_dict[a])
    dir_b = Path.join(Path.parent(data_dir), key_name_dict[b])
    dir_c = Path.join(Path.parent(data_dir), key_name_dict[c])
    dir_d = Path.join(Path.parent(data_dir), key_name_dict[d])
    
    Path.makeDirs(dir_a)
    Path.makeDirs(dir_b)
    Path.makeDirs(dir_c)
    Path.makeDirs(dir_d)
    
    file_names = Path.listImg(data_dir, return_path=False)
    setWindowCV("show", size=(720, 480))
    for fn in file_names:
        fp = Path.join(data_dir, fn)
        print(fp)
        img = cv2.imread(fp)
        again = True
        while again:
            k = loopShowCV("show", img)
            if k & 0xff == ord(a):
                Path.move(fp, dir_a)
                Path.rename(Path.join(dir_a, fn), Path.join(dir_a, data_name + "_" + fn))
                again = False
            elif k & 0xff == ord(b):
                Path.move(fp, dir_b)
                Path.rename(Path.join(dir_b, fn), Path.join(dir_b, data_name + "_" + fn))
                again = False
            elif k & 0xff == ord(c):
                Path.move(fp, dir_c)
                Path.rename(Path.join(dir_c, fn), Path.join(dir_c, data_name + "_" + fn))
                again = False
            elif k & 0xff == ord(d):
                Path.move(fp, dir_d)
                Path.rename(Path.join(dir_d, fn), Path.join(dir_d, data_name + "_" + fn))
                again = False
            else:
                again = True
                print(cs.yellow("选择文件夹错误，请重新输入！"))
            

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

