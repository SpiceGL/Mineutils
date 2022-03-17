import os
import sys
sys.path.append("..")
from mineutils.path import Path
from mineutils.colorstr import ColorStr


def rename_datas():
    """
    对选定的几个数据集重命名内部图像和标签文件名，避免将数据集合并的时候文件名重复。
    通过下划线"_"分离数据集名，选择前缀加在数据文件名前来给数据重命名。
    """
    data_paths = ["../w6013_traindata",
                  "../Z75_traindata",
                  "../ZTC950V763_traindata",
                  "../Z86_traindata"]   ###待重命名的数据集路径
    dirs = ["imgs", "labels"]   ###数据集中需要重命名的文件所在文件夹
    
    print(ColorStr.green("程序正在运行，请等待……"))
    for data_path in data_paths:
        data_name = Path.splitName(data_path)
        prefix = data_name.split("_")[0]
        for d in dirs:
            dir_path = os.path.join(data_path, d)
            file_names = Path.listdir(dir_path, return_path=False)
            file_paths = [os.path.join(dir_path, file_name) for file_name in file_names]
            new_paths = [os.path.join(dir_path, prefix + "_" + file_name) for file_name in file_names]
            
            for i in range(len(file_paths)):
                if file_names[i] != "classes.txt":
                    os.rename(file_paths[i], new_paths[i])


if __name__ == "__main__":
    from mineutils import useProjDir
    useProjDir(splitsrt="my_tools")
    print(sys.path)
