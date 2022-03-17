from mineutils.path import Path
from mineutils import Error
from mineutils.colorstr import ColorStr
import cv2
from collections import OrderedDict


class Pkl:
    @classmethod
    def save(cls, save_path: str, save_dict: dict):
        """
        --保存字典为pkl格式文件。
        """
        import pickle as pkl
        with open(save_path, "wb") as f:
            pkl.dump(save_dict, f, pkl.HIGHEST_PROTOCOL)
            
    @classmethod
    def load(cls, path: str) -> dict:
        """
        --读取pkl格式文件为字典。
        """
        import pickle as pkl
        with open(path, "rb") as f:
            loaded_dict = pkl.load(f)
        return loaded_dict
    

class Yaml:
    @classmethod
    def save(cls, save_path: str, save_dict: dict):
        """
        --保存字典为yaml格式文件。
        --只能保存python基本数据类型，不能保存numpy的float64等类型，
        """
        import yaml
        with open(save_path, "w", encoding="utf-8") as f:
            yaml.dump(save_dict, f)
    
    @classmethod
    def load(cls, path: str) -> dict:
        """
        --读取yaml格式文件为字典。
        """
        import yaml
        with open(path, "r") as f:
            content = f.read()
            loaded_dict = yaml.load(content, Loader=yaml.FullLoader)
        return loaded_dict

    @classmethod
    def renameKey(cls, path, old_key, new_key):
        """
        --输入yaml文件路径，对文件内key进行重命名。
        --只处理一级的key，如dict[key]，不能处理dict[key1][key2]...。
        """
        if isinstance(path, str):
            content = Yaml.load(path)
            if old_key in content:
                content[new_key] = content.pop(old_key)
            else:
                raise Error("'old_key'参数有误！")
            Yaml.save(path, content)
        else:
            raise Error("输入类型不正确！")
        
    @classmethod
    def removeKey(cls, path, key, ):
        """
        --输入yaml文件路径，对文件内key进行重命名。
        --只处理一级的key，如dict[key]，不能处理dict[key1][key2]...。
        """
        if isinstance(path, str):
            content = Yaml.load(path)
            if key in content:
                content.pop(key)
            else:
                raise Error("'key'参数有误！")
            Yaml.save(path, content)
        else:
            raise Error("输入类型不正确！")

    @classmethod
    def updataValue(cls, path, key, new_value):
        """
        --输入yaml文件路径，对文件内key进行重命名。
        --只处理一级的key，如dict[key]，不能处理dict[key1][key2]...。
        """
        if isinstance(path, str):
            content = Yaml.load(path)
            if key in content:
                content[key] = new_value
            else:
                raise Error("'key'参数有误！")
            Yaml.save(path, content)
        else:
            raise Error("输入类型不正确！")
        
        
    
        
def imgGenerator(input_path, size):
    """
    --为分类网络的效果测试准备数据。
    --
    """
    if Path.isDir(input_path):
        mode = "imgs"
    elif Path.isImage(input_path):
        mode = "predict"
    elif Path.isFile(input_path):
        mode = "video"
    else:
        raise Error("ClsTestLoader(): 请检查input_size是否正确！")
    if mode == "imgs":
        img_paths = Path.listDir()
    
            
if __name__ == "__main__":
    from mineutils import useProjDir
    useProjDir()
    Yaml.updataKey(1, 0, 2)

