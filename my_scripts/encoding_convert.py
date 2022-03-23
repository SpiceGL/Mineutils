import os

import chardet
import codecs
import sys
sys.path.append("..")

from mineutils.colorstr import ColorStr as cs
from mineutils.path import Path as pt


def getEncodingFormat(file_path):
    with open(file_path, "rb") as f:
        content = f.read()
        encoding_format = chardet.detect(content)['encoding']
        if encoding_format is None:
            print(cs.red("getEncodingFormat:"), "编码格式为None，可能读取错误！")
            exit(0)
        return encoding_format


def convertEncoding(file_path, target_encoding="utf-8"):
    past_encoding = getEncodingFormat(file_path)
    with codecs.open(file_path, "rb", encoding=past_encoding) as f:
        content = f.read()
    with codecs.open(file_path, "wb", encoding=target_encoding) as f:
        f.write(content)
    print(cs.green("convertEncoding:"), cs.fuchsia(file_path) + f"已从{past_encoding}转换为{target_encoding}！")
        
    
def batchConvert(file_dir, backup_dir, target_encoding, file_extensions=()):
    if pt.exists(backup_dir):
        print(cs.red("batchConvert:"), "备份文件夹已存在，请手动删除避免被误覆盖，程序已中止！")
        return
    pt.copy(pt.join(file_dir, "*"), backup_dir)
    file_paths = pt.walk(file_dir)
    for fp in file_paths:
        if pt.extension(fp) in file_extensions:
            convertEncoding(fp, target_encoding)
            




def main():
    file_dir = "../mineutilshpp"
    backup_dir = file_dir + "_backup"
    target_encoding = "UTF-8-SIG"
    file_extensions = (".py", ".h", ".hpp", ".c", ".cpp")
    batchConvert(file_dir, backup_dir, target_encoding, file_extensions)
    

def showEncoding():
    file_dir = "../mineutilshpp"
    backup_dir = file_dir + "_backup"
    target_encoding = "UTF-8-SIG"
    file_extensions = (".py", ".h", ".hpp", ".c", ".cpp")
    
    file_paths = pt.walk(file_dir)
    for fp in file_paths:
        if pt.extension(fp) in file_extensions:
            past_encoding = getEncodingFormat(fp)
            print(cs.green("convertEncoding:"), cs.fuchsia(fp) + f"的编码格式为{past_encoding}。")


if __name__ == '__main__':
    a = [1,2,3]
    print(a[-1:1])
    

