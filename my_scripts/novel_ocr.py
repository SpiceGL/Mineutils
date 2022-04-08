import textwrap
import cv2
import sys
sys.path.append("..")

from aip import AipOcr
from tqdm import tqdm

from mineutils.colorstr import ColorStr as cs
from mineutils.path import Path as pt


def imgCrop(img_paths, reserve_rate_h=1., reserve_rate_w=1., reserve_mode="middle"):
    crop_size_h = -1
    crop_size_w = -1
    if reserve_mode == "middle":
        for impath in tqdm(img_paths):
            img = cv2.imread(impath)
            if crop_size_h <= 0 and crop_size_w <= 0:
                crop_size_h = int((1 - reserve_rate_h) * img.shape[0])
                crop_size_w = int((1 - reserve_rate_h) * img.shape[1])
            if crop_size_h >= 0 and crop_size_w >= 0:
                img = img[crop_size_h//2 : -crop_size_h//2+1, crop_size_w//2 : -crop_size_w//2+1]
                cv2.imwrite(impath, img)
    


def oneChapterOCR(chapter_dirpath, client):
    img_paths = pt.listDir(chapter_dirpath)
    img_paths.sort()
    line = False
    section = ""
    with open(chapter_dirpath+".txt", "w", encoding="utf8") as txt:
        for impath in tqdm(img_paths):
            with open(impath, 'rb') as f:
                img = f.read()
            text = client.basicAccurate(img)
            for each in text.get('words_result'):
                line = each.get('words')   #章节名
                # txt.write(line)
                if len(line) >= 18:
                    txt.write(line)
                else:
                    txt.write(line)
                    txt.write("\n")


def main_ocr():
    # 在这三个地方填入参数
    api_id = '23638418'
    api_key = '6tIPlZnueQSqzSITpjgH8GQU'
    secret_key = 'rD1EZB5yZrkXXc4AukA2Yj9cx0YnUMZf'
    client = AipOcr(api_id, api_key, secret_key)  # 到这里都是固定用法
    
    novel_dirpath = "E:/Project/005-OCR/novel2"
    chapter_dirpaths = pt.listDir(novel_dirpath)
    print(f"共有{len(chapter_dirpaths)}章内容待识别！")
    for chapter_dirpt in chapter_dirpaths:
        oneChapterOCR(chapter_dirpt, client)
        chapter_id = pt.splitName()
        print(cs.blue(f""))
    

def main_crop():
    img_dirpath = "E:/Project/005-OCR/novel2"
    img_paths = pt.walk(img_dirpath)
    print(f"共有{len(img_paths)}张待识别图片！")
    imgCrop(img_paths, 0.9266, 1)


if __name__ == '__main__':
    main_ocr()
    
    
