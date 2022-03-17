from mineutils.colorstr import ColorStr
from mineutils.path import Path


class NameCreator:
    """
    --解决取名困难症，按照一定规则创建不与当前目录里内容重名的文件/目录名。
    """
    @classmethod
    def createNumberName(cls, dirpath, length=3):
        """
        --输入希望创建的文件/目录所在目录以及name长度，按照序号顺序得到不重名的name。
        """
        file_paths = Path.glob(Path.join(dirpath, "[0-9]"*length))
        file_paths.sort()
        if file_paths:
            new_name = str(int(Path.splitName(file_paths[-1])) + 1).zfill(length)
            if len(new_name) > length:
                print(ColorStr.yellow("NameCreator.createNumberName():"), "命名长度溢出！")
        else:
            new_name = "0".zfill(length)
        return new_name
