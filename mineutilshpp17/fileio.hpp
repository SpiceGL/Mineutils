#pragma once
#include<fstream>
#include<iostream>
#include<string>


//void cvMatIntoFile(cv::Mat M, string filepath)
//{
//	//使用ofstream来输出mat
//	std::ofstream Fs(filepath);
//	if (!Fs.is_open())
//	{
//		std::cout << "open file error!" << std::endl;
//		return;
//	}
//
//	int height = M.rows;
//	int width = M.cols;
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			Fs << (float)M.ptr<float>(i)[j] << '\t';
//			//不加类型转换用txt打开是字符
//		}
//		Fs << std::endl;
//	}
//	Fs.close();
//	cout << "已将Mat内容写入文件" << filepath << endl;
//}