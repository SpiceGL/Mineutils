#pragma once
#include<string>

#include<opencv2/opencv.hpp>
#include<limits.h>
#include<stdlib.h>

#include"colorstr.hpp"
#include"index.hpp"
#include"print.hpp"
#include"sign.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::string;


/*------------------------------------声明-------------------------------------*/
string setWindowCV(string win_name, cv::Size size, pair<int, int> position, int flag);
bool loopShowCV(string win_name, cv::Mat& img, float wait);
void quickShowCV(string win_name, cv::Mat& img, float wait, bool close, cv::Size size, pair<int, int> position, int flag);
void quickPlayCV(string win_name, string video_path, float wait, cv::Size size, pair<int, int> position, int flag);

template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
void printMat(cv::Mat& img, const Tx& x_range, const Ty& y_range, const Tc& c_range);

template<class Tx = pair<int, int>, class Ty = pair<int, int>>
void printMat(cv::Mat& img, const Tx& x_range, const Ty& y_range);

template<class Tx = pair<int, int>>
void printMat(cv::Mat& img, const Tx& x_range);

void printMat(cv::Mat& img);

template<class MatT>
void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend);

template<class MatT>
void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend, int cstart, int cend);


/*------------------------------------定义-------------------------------------*/
//快速设置窗口属性
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

//快速显示图像，窗口属性在函数外设置，推荐用于循环体中，若收到中止信号，则返回false
bool loopShowCV(string win_name, cv::Mat& img, float wait = 1)
{
	cv::imshow(win_name, img);
	int k = cv::waitKey(wait) & 0xff;
	if (k == 27 or k == int('q'))
		return false;
	else
		return true;
}



//快速显示图像，一步到位设置窗口和显示属性
void quickShowCV(string win_name, cv::Mat& img, 
	float wait = 0, bool close = true, cv::Size size = {-1, -1}, 
	pair<int, int> position = {-1, -1}, int flag = cv::WINDOW_FREERATIO)
{
	using cs = ColorStr;
	if (img.empty())
		print(cs::yellow(__func__, ":"), "图像打开失败，已跳过显示！");
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

//快速显示视频
void quickPlayCV(string win_name, string video_path, 
	float wait = 30, cv::Size size = { -1, -1 }, pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO)
{
	using cs = ColorStr;
	auto cap = cv::VideoCapture(video_path);
	if (not cap.isOpened())
	{
		print(cs::yellow(__func__, ":"), "视频文件打开失败，已跳过播放！");
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
			print(cs::green(__func__, ":"), "视频已播放结束。");
			break;
		}
		bool go_on = loopShowCV(win_name, frame, wait);
		if (not go_on)
		{
			cv::destroyWindow(win_name);
			print(cs::yellow(__func__, ":"), "收到中止信号，提前结束视频播放！");
			break;
		}
	}
	cap.release();
}

/*---------------------------------------------------------------------------------*/



template<class Tx, class Ty, class Tc>
void printMat(cv::Mat& img, const Tx& x_range, const Ty& y_range, const Tc& c_range)
{
	/*		C1	C2	C3	C4
	CV_8U	0	8	16	24
	CV_8S	1	9	17	25
	CV_16U	2	10	18	26
	CV_16S	3	11	19	27
	CV_32S	4	12	20	28
	CV_32F	5	13	21	29
	CV_64F	6	14	22	30 */
	using cs = ColorStr;

	pair<int, int> x_norm_range = normRange(x_range, img.cols);
	pair<int, int> y_norm_range = normRange(y_range, img.rows);
	pair<int, int> c_norm_range = normRange(c_range, img.channels());
	int xstart = x_norm_range.first, xend = x_norm_range.second;
	int ystart = y_norm_range.first, yend = y_norm_range.second;
	int cstart = c_norm_range.first, cend = c_norm_range.second;
	print(xstart, xend, ystart, yend, cstart, cend);
	if (img.type() == 0) 
		_printCVMat<uchar>(img, xstart, xend, ystart, yend);
	else if (img.type() == 16) 
		_printCVMat<cv::Vec3b>(img, xstart, xend, ystart, yend, cstart, cend);
	else if (img.type() == 21) 
		_printCVMat<cv::Vec3f>(img, xstart, xend, ystart, yend, cstart, cend);
	else print(cs::yellow(__func__, ":"), "该图像的cv::Mat::type()暂不支持，已跳过输出!");
}

template<class Tx, class Ty>
void printMat(cv::Mat& img, const Tx& x_range, const Ty& y_range)
{
	printMat(img, x_range, y_range, sign::ALL);
}

template<class Tx>
void printMat(cv::Mat& img, const Tx& x_range)
{
	printMat(img, x_range, sign::ALL, sign::ALL);
}

void printMat(cv::Mat& img)
{
	printMat(img, sign::ALL, sign::ALL, sign::ALL);
}




template<class MatT>
void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend)
{
	if (yend - ystart == 1) 
		cout << "cv::Mat{";
	else cout << "cv::Mat{" << endl;

	for (int y = ystart; y < yend; y++)
	{
		cout << "[";
		for (int x = xstart; x < xend; x++)
		{
			if (x != xend - 1) 
				cout << std::setw(cout_width) << std::setprecision(cout_width-1) << (int)img(y, x) << " ";
			else cout << std::setw(cout_width) << std::setprecision(cout_width-1) << (int)img(y, x);
		}
		if (y != yend - 1) 
			cout << "]" << endl;
		else cout << "]";
	}
	cout << "}" << endl;
}

template<class MatT>
void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend, int cstart, int cend)
{
	if (yend - ystart == 1) 
		cout << "cv::Mat{";
	else cout << "cv::Mat{" << endl;

	for (int y = ystart; y < yend; y++)
	{
		cout << "[";
		for (int x = xstart; x < xend; x++)
		{
			cout << "(";
			for (int c = cstart; c < cend; c++)
			{
				if (c != cend - 1) 
					cout << std::setw(4) << std::setprecision(3) << (int)img(y, x)[c] << " ";
				else cout << std::setw(4) << std::setprecision(3) << (int)img(y, x)[c];
			}
			if (x != xend - 1) 
				cout << ") ";
			else cout << ")";
		}
		if (y != yend - 1) 
			cout << "]" << endl;
		else cout << "]";
	}
	cout << "}" << endl;
}
