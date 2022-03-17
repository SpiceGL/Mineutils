import sys
import os

now_path = os.getcwd()
proj_path = os.path.normpath(now_path.split("mineutils")[0])
if now_path in sys.path:
    sys.path.remove(now_path)
if proj_path not in sys.path:
    sys.path.append(proj_path)
