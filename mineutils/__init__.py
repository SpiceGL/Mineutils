import sys
import os


now_path = os.getcwd()
proj_path = os.path.normpath(now_path.split("mineutils")[0])
if now_path in sys.path:
    sys.path.remove(now_path)
if proj_path not in sys.path:
    sys.path.append(proj_path)


def useProjDir(splitsrt="mineutils"):
    proj_path = os.path.normpath(now_path.split(splitsrt)[0])
    os.chdir(proj_path)


class Error(Exception):
    """
    --主动设置错误信息。
    """
    def __init__(self, message: str):
        pass
    

