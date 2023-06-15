# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 14:07:45 2021

@author: 00769901
"""
import os
import shutil
import glob
import stat

from mineutils.colorstr import ColorStr


class Path:
    """
    --解决os中一些操作不便利的问题。
    --整合os、glob、shutil等模块的文件路径相关操作。
    """
    sep = os.path.sep

    @classmethod
    def abs(cls, path: str):
        """
        --获取绝对路径。
        """
        path = os.path.abspath(path)
        return cls.normalPath(path)
    
    @classmethod
    def chWorkingDir(cls, path: str):
        """
        --改变当前文件的运行目录。
        """
        path = cls.normalPath(path)
        os.chdir(path)
        
    @classmethod
    def commonPath(cls, *paths: str) -> str:
        """
        --获取一堆路径组成的列表的最长公共文件夹路径。
        """
        paths = list(paths)
        com_path = os.path.commonpath(paths)
        return cls.normalPath(com_path)
    
    @classmethod
    def copy(cls, wildcard_path: str, new_dir: str):
        """
        --将符合要求的文件/文件夹复制到新文件夹下。
        --wildcard_path是可以用通配符表示的一类文件/文件夹。
        """
        wildcard_path = cls.normalPath(wildcard_path)
        new_dir = cls.normalPath(new_dir)
        files_waiting = glob.glob(wildcard_path)   ###得到的是路径
        if not files_waiting:
            print(ColorStr.yellow("Path.copy:"), f"{wildcard_path}不存在，因此未复制任何文件！")
        else:
            if not os.path.exists(new_dir):
                os.makedirs(new_dir)
            for file in files_waiting:
                file_name = cls.splitName(file)
                new_path = os.path.join(new_dir, file_name)
                if os.path.isfile(file):
                    shutil.copy(file, new_path)
                elif os.path.isdir(file):
                    shutil.copytree(file, new_path, dirs_exist_ok=True)
                    
    @classmethod
    def cwd(cls):
        """
        --返回当前工作目录的绝对路径。
        """
        return cls.normalPath(os.getcwd())
    
    @classmethod
    def exists(cls, path):
        """
        --判断路径是否真实存在。
        """
        return os.path.exists(path)
    
    @classmethod
    def extension(cls, path):
        """
        --获取文件的后缀名(带.号)
        """
        return os.path.splitext(path)[-1]
    
    @classmethod
    def glob(cls, wildcard_path: str) -> list:
        """
        --通过通配符找到符合要求的路径，但通配符不能代表路径分隔符。
        --*号表示0或多个字符，?号代表一个字符，[0-9]代表一个范围内的单字符
        """
        return glob.glob(wildcard_path)

    @classmethod
    def isDir(cls, path: str) -> bool:
        """
        --判断路径是否是一个真实文件夹。
        """
        return os.path.isdir(path)
    
    @classmethod
    def isFile(cls, path: str) -> bool:
        """
        --判断路径是否是一个真实文件。
        """
        return os.path.isfile(path)
    
    @classmethod
    def isImage(cls, path: str) -> bool:
        """
        ----判断路径是否是一个真实图像文件。
        """
        return os.path.isfile(path) and cls.isImageType(path)

    @classmethod
    def isVideo(cls, path: str) -> bool:
        """
        ----判断路径是否是一个真实视频文件。
        """
        return os.path.isfile(path) and cls.isVideoType(path)
    
    @classmethod
    def isImageType(cls, name: str) -> bool:
        """
        --根据字符串结尾判断字符串是否是合法的图像格式名。
        """
        return any(name.endswith(extension) for extension in [".png", ".jpg", ".jpeg", ".PNG", ".JPG", ".JPEG"])

    @classmethod
    def isVideoType(cls, name: str) -> bool:
        """
        --根据字符串结尾判断字符串是否是合法的视频格式名。
        """
        return any(name.endswith(extension) for extension in ["avi", "AVI", "mp4", "MP4", "flv", "FLV"])
        
    @classmethod
    def join(cls, *strs):
        s = strs[0]
        if strs[0][-1] == ":":
            strs = list(strs)
            strs[0] += "/"
        path = os.path.join(*strs)
        return cls.normalPath(path)

    @classmethod
    def listDir(cls, dirpath: str, ignore_names=None, return_path=True) -> list:
        """
        --输入文件夹路径，返回该文件夹下所有一级文件/文件夹的路径。
        """
        dirpath = cls.normalPath(dirpath)
        names = os.listdir(dirpath)
        if ignore_names:
            names = [name for name in names if name not in ignore_names]
            
        if return_path:
            paths = [cls.join(dirpath, name) for name in names]
            return paths
        else:
            return names

    @classmethod
    def listImg(cls, dirpath: str, ignore_names=(), return_path=True) -> list:
        """
        --输入文件夹路径，返回该文件夹下所有一级图像的路径。
        """
        names = os.listdir(dirpath)
        if ignore_names:
            for ignore_name in ignore_names:
                while ignore_name in names:
                    names.remove(ignore_name)
    
        if return_path:
            img_paths = [cls.join(dirpath, name) for name in names if cls.isImage(name)]
            return img_paths
        else:
            img_names = [name for name in names if cls.isImageType(name)]
            return img_names

    @classmethod
    def makeDirs(cls, dirpath: str):
        """
        --如果输入目录不存在，则创建目录。
        """
        if not os.path.exists(dirpath):
            os.makedirs(dirpath)
        else:
            print(ColorStr.yellow("Path.makeDirs():"), f"{ColorStr.fuchsia(dirpath)}已存在，因此未创建新的文件夹！")
            
    @classmethod
    def move(cls, wildcard_path: str, new_dir: str):
        """
        --将符合要求的文件或文件夹移动到新文件夹下。
        --wildcard_path是可以用通配符表示的一类文件/文件夹。
        """
        files_waiting = glob.glob(wildcard_path)   ###得到的是路径
        if not files_waiting:
            print(ColorStr.yellow("Path.move:"), f"{wildcard_path}不存在，因此未移动任何文件！")
        else:
            if not os.path.exists(new_dir):
                os.makedirs(new_dir)
            for file in files_waiting:
                file_name = cls.splitName(file)
                new_path = os.path.join(new_dir, file_name)
                shutil.move(file, new_path)
    
    @classmethod
    def normalPath(cls, path: str) -> str:
        """
        --获得输入路径的规范写法。
        """
        path = os.path.normpath(path)
        str_list = path.split("\\")
        s = str_list[0]
        for sl in str_list[1:]:
            if len(sl) > 0:
                s += "/"
                s += sl
        return s
    
    @classmethod
    def parent(cls, path: str, use_abspath=False) -> str:
        """
        --获取给定路径的父目录。
        --系统根目录的父目录是自身。
        """
        path = cls.normalPath(path)
        abspath = cls.normalPath(os.path.abspath(path))
        absparent = cls.normalPath(os.path.dirname(abspath))
        # if absparent[-1] == ":":
        #     absparent += "/"
        if not use_abspath:
            work_path = cls.normalPath(os.getcwd())
            level_work = len(work_path.split("/"))   ###从根目录到工作目录的级数
            level_absparent = len(absparent.split("/"))
            if level_work > level_absparent:
                pp_list = [".."] * (level_work-level_absparent)
                parent_path = os.path.join(*pp_list)   ###父目录超出工作目录后，相对路径的父目录写法
            else:
                parent_path = os.path.dirname(path)
            return parent_path
        else:
            return absparent
            
    @classmethod
    def splitName(cls, path: str, suffix=True) -> str:
        """
        --从路径中分离出不带后缀名的文件名。
        """
        if path[-1] == "/":
            # print(ColorStr.yellow("Path.splitName()"), f"{path}值最后一位是'/'！")
            # raise Error("111")
            path = path[0:-1]
        if suffix:
            return os.path.basename(path)
        else:
            return os.path.splitext(os.path.basename(path))[0]

    @classmethod
    def remove(cls, path: str):
        """
        --删除输入文件或文件夹。
        """
        if os.path.isfile(path):
            os.remove(path)
        elif os.path.isdir(path):
            shutil.rmtree(path, onerror=cls._removeReadonly)
        else:
            print(ColorStr.yellow("Path.remove:"), f"{path}不存在，因此未删除任何文件！")
    
    @classmethod
    def _removeReadonly(cls, func, path, execinfo):
        os.chmod(path, stat.S_IWUSR)  # 修改文件权限
        func(path)

    @classmethod
    def rename(cls, path: str, new_name_path: str):
        """
        --修改目录或文件名
        """
        if cls.parent(path) != cls.parent(new_name_path):
            print(ColorStr.red("Path.rename:"), f"{path}与目标路径{new_name_path}应在同一目录下！程序已中止！")
            exit(0)
        if not os.path.isdir(path) and not os.path.isfile(path):
            print(ColorStr.yellow("Path.rename:"), f"{path}不存在，因此未重命名任何文件！")
        else:
            shutil.move(path, new_name_path)
                
    @classmethod
    def walk(cls, dirpath, return_path=True):
        """
        --遍历目录下所有文件
        """
        all_files = []
        for root, dirs, files in os.walk(dirpath):
            if return_path:
                [all_files.append(os.path.join(root, f)) for f in files]
            else:
                all_files.extend(files)
        return all_files


if __name__ == '__main__':
    # print(Path.commonPath("aaa/bbb", "aaa/ccc"))
    # print(os.getenv("SYSTEMROOT"))
    print(Path.sep)

        
    



