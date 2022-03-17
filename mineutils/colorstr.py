# -*- coding: utf-8 -*-
"""
Created on Wed Sep 29 16:18:14 2021
@author: 00769901
"""

class ColorStr(object):
    """
    --在print中显示带颜色的字体。
    """
    @classmethod
    def blue(cls, s: str) -> str:
        """
        --常用于重点信息打印。
        """
        return "\033[34m" + s + "\033[0m"
    
    @classmethod
    def cyan(cls, s: str) -> str:
        return "\033[36m" + s + "\033[0m"
    
    @classmethod
    def fuchsia(cls, s: str) -> str:
        """
        --常用于关键词标红。
        """
        return "\033[35m" + s + "\033[0m"
    
    @classmethod
    def green(cls, s: str) -> str:
        """
        --常用于成功/通过信息打印。
        """
        return "\033[32m" + s + "\033[0m"
  
    @classmethod
    def red(cls, s: str) -> str:
        """
        --常用于中断/不通过信息打印。
        """
        return "\033[31m" + s + "\033[0m"

    @classmethod
    def yellow(cls, s: str) -> str:
        """
        --常用于警告信息。
        """
        return "\033[33m" + s + "\033[0m"


  

  

  
    # @classmethod
    # def white(cls, s: str) -> str:
    #     return cls._color_str('_WHITE', s)


if __name__ == "__main__":
    from time import time
    start = time()
    i = 0
    for a in range(100000000):
        i += 1
    print(i, time()-start)