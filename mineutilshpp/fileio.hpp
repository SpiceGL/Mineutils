#pragma once
#include<fstream>
#include<iostream>
#include<string>


//void cvMatIntoFile(cv::Mat M, string filepath)
//{
//	//ʹ��ofstream�����mat
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
//			//��������ת����txt�����ַ�
//		}
//		Fs << std::endl;
//	}
//	Fs.close();
//	cout << "�ѽ�Mat����д���ļ�" << filepath << endl;
//}