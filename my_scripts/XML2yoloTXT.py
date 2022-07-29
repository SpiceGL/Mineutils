import os.path
import xml.etree.ElementTree as ET
import sys

from sqlalchemy.orm import class_mapper

sys.path.append("E://MineLibs")
from mineutils.path import Path


def makeYoloTXT(xml_path, txt_path, class_names):
    files = Path.listDir(xml_path, return_path=False)
    i = 0
    while i < len(files):
        
        name = files[i].split(".")[0]
        xml_name = name + ".xml"
        txt_name = name + ".txt"
        xml_file_path = Path.join(xml_path, xml_name)
        txt_file_path = Path.join(txt_path, txt_name)
        print(xml_file_path)
        xml_file = open(xml_file_path)
        tree = ET.parse(xml_file)
        root = tree.getroot()
        filename = root.find('filename').text
        
        image_name = root.find('filename').text
        w = int(root.find('size').find('width').text)
        h = int(root.find('size').find('height').text)
        
        f_txt = open(txt_file_path, 'w+')
        content = ""
        
        first = True
        
        for obj in root.iter('object'):
            
            name = obj.find('name').text
            class_id = class_names.index(name)
            
            ###将几类合并为一类
            if class_id == 3:
                class_id = 1
            elif class_id == 4:
                class_id = 2
            elif class_id == 7 or class_id == 8:
                class_id = 3
            else:
                class_id = 0
            
            xmlbox = obj.find('bndbox')
            
            x1 = int(xmlbox.find('xmin').text)
            x2 = int(xmlbox.find('xmax').text)
            y1 = int(xmlbox.find('ymin').text)
            y2 = int(xmlbox.find('ymax').text)
            
            if first:
                content += str(class_id) + " " + \
                           str((x1 + x2) / 2 / w) + " " + str((y1 + y2) / 2 / h) + " " + \
                           str((x2 - x1) / w) + " " + str((y2 - y1) / h)
                first = False
            else:
                content += "\n" + \
                           str(class_id) + " " + \
                           str((x1 + x2) / 2 / w) + " " + str((y1 + y2) / 2 / h) + " " + \
                           str((x2 - x1) / w) + " " + str((y2 - y1) / h)
        
        # print(str(i / (number - 1) * 100) + "%\n")
        # print(content)
        f_txt.write(content)
        f_txt.close()
        xml_file.close()
        i += 1
    
    print("done!")


def main():
    class_names = ['Weed_1', 'Weed_2', 'Weed_3', 'Weed_4', 'Weed_5', 'Weed_6', 'Weed_7', 'Weed_8', 'Weed_9', 'Weed_10']
    
    xmlpath = r'E:\Project\007-农业辅助作业\weedsdata\xmls\train'  # 原xml路径
    txtpath = r'E:\Project\007-农业辅助作业\weedsdata\labels\train'  # 转换后txt文件存放路径
    if not Path.exists(txtpath):
        Path.makeDirs(txtpath)
    
    makeYoloTXT(xmlpath, txtpath, class_names)


if __name__ == '__main__':
    main()