#pragma once
#include<string>

#include<opencv2/opencv.hpp>
#include<limits.h>
#include<stdlib.h>

#include"bbox.hpp"
#include"colorstr.hpp"
#include"index.hpp"
#include"print.hpp"
#include"sign.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::string;

namespace mineutils
{
	/*------------------------------------声明-------------------------------------*/
	string setWindowCV(string win_name, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	bool loopShowCV(string win_name, cv::Mat& img, float wait = 1);

	void quickShowCV(string win_name, cv::Mat& img,
		float wait = 0, bool close = true, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void quickPlayCV(string win_name, string video_path,
		float wait = 30, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void putLabel(cv::Mat& img, string label, cv::Point position,
		cv::Scalar bg_color = { 0, 255, 0 }, cv::Scalar text_color = { 255,255,255 },
		int text_thickness = 2);

	void putBbox(cv::Mat& img, LTRB ltrb, string label = "",
		cv::Scalar bbox_color = { 0,255,0 }, cv::Scalar text_color = { 255,255,255 },
		int bbox_thickness = 3, int text_thickness = 2);

	template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
	void printMat(cv::Mat& img, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });

	template<class MatT>
	void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend);

	template<class cvVec>
	void _printCVMat(cv::Mat_<cvVec> img, int xstart, int xend, int ystart, int yend, int cstart, int cend);


	/*------------------------------------定义-------------------------------------*/
	//快速设置窗口属性
	string setWindowCV(string win_name, cv::Size size, pair<int, int> position, int flag)
	{
		cv::namedWindow(win_name, flag);
		if (size.width != -1)
			cv::resizeWindow(win_name, size);
		if (position.first != -1)
			cv::moveWindow(win_name, position.first, position.second);
		return win_name;
	}

	//快速显示图像，窗口属性在函数外设置，推荐用于循环体中，若收到中止信号，则返回false
	bool loopShowCV(string win_name, cv::Mat& img, float wait)
	{
		cv::imshow(win_name, img);
		int k = cv::waitKey(wait) & 0xff;
		if (k == 27 or k == int('q'))
			return false;
		else
			return true;
	}


	//快速显示图像，一步到位设置窗口和显示属性
	void quickShowCV(string win_name, cv::Mat& img, float wait, bool close,
		cv::Size size, pair<int, int> position, int flag)
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
	void quickPlayCV(string win_name, string video_path, float wait,
		cv::Size size, pair<int, int> position, int flag)
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
	void putLabel(cv::Mat& img, string label, cv::Point position,
		cv::Scalar bg_color, cv::Scalar text_color, int text_thickness)
	{
		if (label.size() != 0)
		{
			int* baseline = nullptr;
			cv::Size text_size = cv::getTextSize(label, 0, text_thickness / 3., text_thickness, baseline);
			cv::Point c1 = position;
			cv::Point c2 = { position.x + text_size.width, position.y - text_size.height };
			cv::rectangle(img, c1, c2, bg_color, -1);
			cv::putText(img, label, c1, 0, text_thickness / 3., text_color, text_thickness, cv::LINE_AA);
		}
	}

	void putBbox(cv::Mat& img, LTRB ltrb, string label,
		cv::Scalar bbox_color, cv::Scalar text_color, int bbox_thickness, int text_thickness)
	{
		cv::Point c1 = { ltrb.left , ltrb.top };
		cv::Point c2 = { ltrb.right , ltrb.bottom };
		cv::rectangle(img, c1, c2, bbox_color, bbox_thickness);

		c1.x -= bbox_thickness - 1;
		c1.y -= bbox_thickness - 1;
		putLabel(img, label, c1, bbox_color, text_color, text_thickness);
	}

	/*---------------------------------------------------------------------------------*/


	template<class Tx, class Ty, class Tc>
	void printMat(cv::Mat& img, Tx x_range, Ty y_range, Tc c_range)
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
		using pt = pair<int, int>;

		pt x_norm_range = normRange(x_range, img.cols);
		pt y_norm_range = normRange(y_range, img.rows);
		pt c_norm_range = normRange(c_range, img.channels());
		int xstart = x_norm_range.first, xend = x_norm_range.second;
		int ystart = y_norm_range.first, yend = y_norm_range.second;
		int cstart = c_norm_range.first, cend = c_norm_range.second;

		if (img.type() == 0)
			_printCVMat<uchar>(img, xstart, xend, ystart, yend);
		else if (img.type() == 16)
			_printCVMat<cv::Vec3b>(img, xstart, xend, ystart, yend, cstart, cend);
		else if (img.type() == 21)
			_printCVMat<cv::Vec3f>(img, xstart, xend, ystart, yend, cstart, cend);
		else if (img.type() == 22)
			_printCVMat<cv::Vec3d>(img, xstart, xend, ystart, yend, cstart, cend);
		else print(cs::yellow(__func__, ":"), "该图像的cv::Mat::type()暂不支持，已跳过输出!");
	}


	template<class MatT>
	void _printCVMat(cv::Mat_<MatT> img, int xstart, int xend, int ystart, int yend)
	{
		using namespace cv;
		using cs = ColorStr;
		int coutw, coutprec;
		if (isInTypes<MatT, uchar>())
			coutw = 3, coutprec = 2;
		else if (isInTypes<MatT, float, double>())
			coutw = 5, coutprec = 4;
		else
		{
			print(cs::yellow(__func__, ":"), "暂未优化的类型，因此未调整输出宽度！");
			coutw = 0, coutprec = 6;
		}
		if (yend - ystart == 1)
			cout << "cv::Mat{";
		else cout << "cv::Mat{" << endl;

		for (int y = ystart; y < yend; y++)
		{
			cout << "[";
			for (int x = xstart; x < xend; x++)
			{
				if (x != xend - 1)
					cout << std::setw(coutw) << std::setprecision(coutprec) << (int)img(y, x) << " ";
				else cout << std::setw(coutw) << std::setprecision(coutprec) << (int)img(y, x);
			}
			if (y != yend - 1)
				cout << "]" << endl;
			else cout << "]";
		}
		cout << "}" << endl;
	}

	template<class cvVec>
	void _printCVMat(cv::Mat_<cvVec> img, int xstart, int xend, int ystart, int yend, int cstart, int cend)
	{
		using namespace cv;
		using cs = ColorStr;
		int coutw, coutprec;
		if (isInTypes<cvVec, Vec2b, Vec3b, Vec4b>())
			coutw = 3, coutprec = 2;
		else if (isInTypes<cvVec, Vec2f, Vec3f, Vec4f, Vec2d, Vec3d, Vec4d>())
			coutw = 5, coutprec = 4;
		else
		{
			print(cs::yellow(__func__, ":"), "暂未优化的类型，因此未调整输出宽度！");
			coutw = 0, coutprec = 6;
		}
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
						cout << std::setw(coutw) << std::setprecision(coutprec) << (int)img(y, x)[c] << " ";
					else cout << std::setw(coutw) << std::setprecision(coutprec) << (int)img(y, x)[c];
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

	//std::ostream& operator<<(std::ostream& cout, cv::Mat& img)   //添加对print的支持
	//{
	//	printMat(img);
	//	return cout;
	//}
}
