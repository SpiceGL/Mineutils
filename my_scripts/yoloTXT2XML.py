# -*- coding: utf-8 -*-
"""
Created on Mon Aug 30 10:49:17 2021

@author: 00769901
"""

from xml.dom.minidom import Document
import os
import cv2
import sys
sys.path.append("..")
from mineutils.path import Path


def makexml(txt_path, xml_path, pic_path, class_dict):  # 读取txt路径，xml保存路径，数据集图片所在路径
    Path.makeDirs(xml_path)
    files = os.listdir(txt_path)
    print("waiting...")
    for i, name in enumerate(files):
        
        if name == "classes.txt":
            continue
        xmlBuilder = Document()
        annotation = xmlBuilder.createElement("annotation")  # 创建annotation标签
        xmlBuilder.appendChild(annotation)
        txtFile = open(os.path.join(txt_path, name))
        txtList = txtFile.readlines()
        img = cv2.imread(os.path.join(pic_path, name[0:-4] + ".jpg"))
        Pheight, Pwidth, Pdepth = img.shape
        
        for j in txtList:
            oneline = j.strip().split(" ")
            
            folder = xmlBuilder.createElement("folder")  # folder标签
            folderContent = xmlBuilder.createTextNode("VOC2007")
            folder.appendChild(folderContent)
            annotation.appendChild(folder)
            
            filename = xmlBuilder.createElement("filename")  # filename标签
            filenameContent = xmlBuilder.createTextNode(name[0:-4] + ".jpg")
            filename.appendChild(filenameContent)
            annotation.appendChild(filename)
            
            size = xmlBuilder.createElement("size")  # size标签
            width = xmlBuilder.createElement("width")  # size子标签width
            widthContent = xmlBuilder.createTextNode(str(Pwidth))
            width.appendChild(widthContent)
            size.appendChild(width)
            height = xmlBuilder.createElement("height")  # size子标签height
            heightContent = xmlBuilder.createTextNode(str(Pheight))
            height.appendChild(heightContent)
            size.appendChild(height)
            depth = xmlBuilder.createElement("depth")  # size子标签depth
            depthContent = xmlBuilder.createTextNode(str(Pdepth))
            depth.appendChild(depthContent)
            size.appendChild(depth)
            annotation.appendChild(size)
            
            the_object = xmlBuilder.createElement("object")
            picname = xmlBuilder.createElement("name")
            nameContent = xmlBuilder.createTextNode(class_dict[oneline[0]])
            picname.appendChild(nameContent)
            the_object.appendChild(picname)
            pose = xmlBuilder.createElement("pose")
            poseContent = xmlBuilder.createTextNode("Unspecified")
            pose.appendChild(poseContent)
            the_object.appendChild(pose)
            truncated = xmlBuilder.createElement("truncated")
            truncatedContent = xmlBuilder.createTextNode("0")
            truncated.appendChild(truncatedContent)
            the_object.appendChild(truncated)
            difficult = xmlBuilder.createElement("difficult")
            difficultContent = xmlBuilder.createTextNode("0")
            difficult.appendChild(difficultContent)
            the_object.appendChild(difficult)
            bndbox = xmlBuilder.createElement("bndbox")
            xmin = xmlBuilder.createElement("xmin")
            mathData = round(((float(oneline[1])) * Pwidth) - (float(oneline[3])) * 0.5 * Pwidth)
            xminContent = xmlBuilder.createTextNode(str(mathData))
            xmin.appendChild(xminContent)
            bndbox.appendChild(xmin)
            ymin = xmlBuilder.createElement("ymin")
            mathData = round(((float(oneline[2])) * Pheight) - (float(oneline[4])) * 0.5 * Pheight)
            yminContent = xmlBuilder.createTextNode(str(mathData))
            ymin.appendChild(yminContent)
            bndbox.appendChild(ymin)
            xmax = xmlBuilder.createElement("xmax")
            mathData = round(((float(oneline[1])) * Pwidth) + (float(oneline[3])) * 0.5 * Pwidth)
            xmaxContent = xmlBuilder.createTextNode(str(mathData))
            xmax.appendChild(xmaxContent)
            bndbox.appendChild(xmax)
            ymax = xmlBuilder.createElement("ymax")
            mathData = round(((float(oneline[2])) * Pheight) + (float(oneline[4])) * 0.5 * Pheight)
            ymaxContent = xmlBuilder.createTextNode(str(mathData))
            ymax.appendChild(ymaxContent)
            bndbox.appendChild(ymax)
            the_object.appendChild(bndbox)
            
            annotation.appendChild(the_object)
        
        f = open(os.path.join(xml_path, name[0:-4] + ".xml"), 'w')
        xmlBuilder.writexml(f, indent='\t', newl='\n', addindent='\t', encoding='utf-8')
        f.close()


if __name__ == '__main__':
    from mineutils import useProjDir
    useProjDir(splitsrt="my_tools")
    txt_path = "../ZTC950V763_20211123/labels"
    xml_path = "../ZTC950V763_20211123/Annotations"
    img_path = "../ZTC950V763_20211123/imgs"
    class_dict = {"0": "normal", "1": "abnormal"}
    
    # print(Path.listdir(".."))
    makexml(txt_path, xml_path, img_path, class_dict)
