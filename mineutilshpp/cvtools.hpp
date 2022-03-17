#pragma once
#include<string>

#include<opencv2/opencv.hpp>
#include<stdlib.h>

#include"colorstr.hpp"
#include"print.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::string;
using cs = ColorStr;


/*------------------------------------����-------------------------------------*/
bool loopShowCV(string win_name, cv::Mat& img, float wait);
void quickShowCV(string win_name, cv::Mat& img, float wait, bool close, cv::Size size, pair<int, int> position, int flag);
void quickPlayCV(string win_name, string video_path, float wait, cv::Size size, pair<int, int> position, int flag);
string setWindowCV(string win_name, cv::Size size, pair<int, int> position, int flag);


/*------------------------------------����-------------------------------------*/
//������ʾͼ�񣬴��������ں��������ã��Ƽ�����ѭ�����У����յ���ֹ�źţ��򷵻�false
bool loopShowCV(string win_name, cv::Mat& img, float wait = 1)
{
	cv::imshow(win_name, img);
	int k = cv::waitKey(wait) & 0xff;
	if (k == 27 or k == int('q'))
		return false;
	else
		return true;
}

//������ʾͼ��һ����λ���ô��ں���ʾ����
void quickShowCV(string win_name, cv::Mat& img, 
	float wait = 0, bool close = true, cv::Size size = {-1, -1}, 
	pair<int, int> position = {-1, -1}, int flag = cv::WINDOW_FREERATIO)
{
	if (img.empty())
		print(cs::yellow(__func__, ":"), "ͼ���ʧ�ܣ���������ʾ��");
	else
	{
		cv::namedWindow(win_name, flag);
		if (size.width != -1)
			cv::resizeWindow(win_name, size);
		if (position.first != -1)
			cv::moveWindow(win_name, position.first, position.second);
		cv::imshow(win_name, img);
		cv::waitKey(wait);
		if (close)
			cv::destroyWindow(win_name);
	}
}

//������ʾ��Ƶ
void quickPlayCV(string win_name, string video_path, 
	float wait = 30, cv::Size size = { -1, -1 }, pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO)
{
	auto cap = cv::VideoCapture(video_path);
	if (not cap.isOpened())
	{
		print(cs::yellow(__func__, ":"), "��Ƶ�ļ���ʧ�ܣ����������ţ�");
		return;
	}
	setWindowCV(win_name, size, position, flag);
	cv::Mat frame;
	while (true)
	{
		bool ret = cap.read(frame);
		if (not ret)
		{
			cv::destroyWindow(win_name);
			print(cs::green(__func__, ":"), "��Ƶ�Ѳ��Ž�����");
			break;
		}
		bool go_on = loopShowCV(win_name, frame, wait);
		if (not go_on)
		{
			cv::destroyWindow(win_name);
			print(cs::yellow(__func__, ":"), "�յ���ֹ�źţ���ǰ������Ƶ���ţ�");
			break;
		}
	}
}

//�������ô�������
string setWindowCV(string win_name, 
	cv::Size size = { -1, -1 }, pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO)
{
	cv::namedWindow(win_name, flag);
	if (size.width != -1)
		cv::resizeWindow(win_name, size);
	if (position.first != -1)
		cv::moveWindow(win_name, position.first, position.second);
	return win_name;
}


template<class cvT>
void _printCVMat(cv::Mat M, pair<int, int> x_range = { 0, 5 }, pair<int, int> y_range = { 0, 5 })
{
	cout << "[";
	for (int y = y_range.first; y < y_range.second; y++)
	{
		for (int x = x_range.first; x < x_range.second; x++)
			cout << M.ptr<cvT>(y)[x] << " ";
		cout << endl;
	}
	cout << "]" << endl;
	cout << "------------------------" << endl;
}

void printCVMat(const cv::Mat& M, pair<int, int> x_range = {0, 5}, pair<int, int> y_range = { 0, 5 })
{
	/*		C1	C2	C3	C4
	CV_8U	0	8	16	24
	CV_8S	1	9	17	25
	CV_16U	2	10	18	26
	CV_16S	3	11	19	27
	CV_32S	4	12	20	28
	CV_32F	5	13	21	29
	CV_64F	6	14	22	30 */

	if (M.type() == 0) _printCVMat<uchar>(M, x_range, y_range);
	else if (M.type() == 16) _printCVMat<cv::Vec3b>(M, x_range, y_range);
	else if (M.type() == 21) _printCVMat<cv::Vec3f>(M, x_range, y_range);
	else
		print(cs::yellow(__func__, ":"), "��ͼ���cv::Mat::type()�ݲ�֧�֣����������!");
}