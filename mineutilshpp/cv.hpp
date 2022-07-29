#pragma once
#include<string>

#include<opencv2/opencv.hpp>
#include<limits.h>
#include<stdlib.h>

#include"points.hpp"
#include"str.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;
	using std::pair;
	using std::string;

	/*------------------------------------声明-------------------------------------*/
	string setWindowCV(string win_name, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	bool loopShowCV(string win_name, cv::Mat& img, float wait = 1);

	void quickShowCV(string win_name, cv::Mat& img,
		float wait = 1, bool close = false, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void quickPlayCV(string win_name, string video_path,
		float wait = 30, cv::Size size = { -1, -1 },
		pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void putLabelCV(cv::Mat& img, string label, cv::Point position, cv::Scalar text_color = { 255,255,255 },
		int word_type = cv::FONT_HERSHEY_SIMPLEX, int word_scale = 1, int text_thickness = 2,
		bool have_bg = true, cv::Scalar bg_color = {255, 0, 0});

	void putBoxCV(cv::Mat& img, LTRB ltrb, string label = "",
		cv::Scalar bbox_color = { 0,255,0 }, cv::Scalar text_color = { 255,255,255 },
		int word_type = cv::FONT_HERSHEY_SIMPLEX, float word_scale = 1, 
		int bbox_thickness = 3, int text_thickness = 2);

	template<class T>
	void channelInit(cv::Mat& mat, cv::Point3_<T> channel_value = {0, 0, 0});

	template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
	void printMat(const cv::Mat& img, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });

	template<class MatT>
	void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, bool isInt);

	template<class cvVec>
	void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, int cstart, int cend, bool isInt);

	void _print(const cv::Mat& img);

	template<class T>
	void _print(const cv::Point_<T>& pt);

	template<class T>
	void _print(const cv::Point3_<T>& pt);

	template<class T>
	void _print(const cv::Size_<T>& sz);

	void _print(const cv::MatSize& sz);

	template<class T>
	void _print(const cv::Rect_<T>& rect);

	/*------------------------------------定义-------------------------------------*/
	
	
	/*快速设置窗口属性*/
	string setWindowCV(string win_name, cv::Size size, pair<int, int> position, int flag)
	{
		cv::namedWindow(win_name, flag);
		if (size.width != -1)
			cv::resizeWindow(win_name, size);
		if (position.first != -1)
			cv::moveWindow(win_name, position.first, position.second);
		return win_name;
	}

	/*快速显示图像，窗口属性在函数外设置，推荐用于循环体中，若收到中止信号，则返回false*/
	bool loopShowCV(string win_name, cv::Mat& img, float wait)
	{
		cv::imshow(win_name, img);
		int k = cv::waitKey(wait) & 0xff;
		bool go_on;
		if (k == 27 or k == int('q'))
			go_on = false;
		else go_on = true;
		return go_on;
	}


	/*快速显示图像，一步到位设置窗口和显示属性*/
	void quickShowCV(string win_name, cv::Mat& img, float wait, bool close,
		cv::Size size, pair<int, int> position, int flag)
	{
		using cs = ColorStr;
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

	/*快速显示视频*/
	void quickPlayCV(string win_name, string video_path, float wait,
		cv::Size size, pair<int, int> position, int flag)
	{
		using cs = ColorStr;
		auto cap = cv::VideoCapture(video_path);
		if (not cap.isOpened())
		{
			cout << cs::yellow(__func__, ":") << " 视频文件打开失败，已跳过播放！" << endl;
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
				cout << cs::green(__func__, ":") << " 视频已播放结束。" << endl;
				break;
			}
			bool go_on = loopShowCV(win_name, frame, wait);
			if (not go_on)
			{
				cv::destroyWindow(win_name);
				cout << cs::yellow(__func__, ":") << "收到中止信号，提前结束视频播放！" << endl;
				break;
			}
		}
		cap.release();
	}


	/*---------------------------------------------------------------------------------*/
	/*为图像添加文字*/
	void putLabelCV(cv::Mat& img, string label, cv::Point position, cv::Scalar text_color, 
		int word_type, float word_scale, int text_thickness, bool have_bg, cv::Scalar bg_color)
	{
		/*	position：文字左下角位置 */
		if (label.size() != 0)
		{
			cv::Point& c1 = position;
			if (have_bg)
			{
				int* baseline = nullptr;
				cv::Size text_size = cv::getTextSize(label, word_type, word_scale, text_thickness, baseline);
				cv::Point c2 = { c1.x + text_size.width, c1.y - (int)(text_size.height*1.2) };
				cv::rectangle(img, c1, c2, bg_color, -1);
			}
			cv::putText(img, label, c1, word_type, word_scale, text_color, text_thickness, cv::LINE_AA);
		}
	}

	/*为图像添加检测框及标签*/
	void putBoxCV(cv::Mat& img, LTRB ltrb, string label,
		cv::Scalar bbox_color, cv::Scalar text_color, int word_type, float word_scale, 
		int bbox_thickness, int text_thickness)
	{
		cv::Point c1 = { ltrb.left , ltrb.top };
		cv::Point c2 = { ltrb.right , ltrb.bottom };
		cv::rectangle(img, c1, c2, bbox_color, bbox_thickness);

		c1.x -= bbox_thickness - 1;
		c1.y -= bbox_thickness - 1;
		
		if (label.size() > 0)
		{
			cv::Point label_pos = c1;
			int* baseline = nullptr;
			cv::Size text_size = cv::getTextSize(label, word_type, word_scale, text_thickness, baseline);
			if (label_pos.x + text_size.width > img.cols)
				label_pos.x = img.cols - text_size.width;
			if (label_pos.y - text_size.height < 0)
				label_pos.y = c1.y + text_size.height;
			putLabelCV(img, label, label_pos, text_color, word_type, word_scale, text_thickness, true, bbox_color);
		}
	}



	/*---------------------------------------------------------------------------------*/
	/*自定义3个通道的值*/
	template<class T>
	void channelInit(cv::Mat& mat, cv::Point3_<T> channel_value)
	{
		vector<cv::Mat> ma_mb_mc;
		cv::split(mat, ma_mb_mc);
		ma_mb_mc[0] = channel_value.x;
		ma_mb_mc[1] = channel_value.y;
		ma_mb_mc[2] = channel_value.z;
		cv::merge(ma_mb_mc, mat);
	}


	/*显示cv::Mat的值*/
	template<class Tx, class Ty, class Tc>
	void printMat(const cv::Mat& img, Tx x_range, Ty y_range, Tc c_range)
	{
		/*		C1	C2	C3	C4
		CV_8U	0	8	16	24       uchar
		CV_8S	1	9	17	25       char
		CV_16U	2	10	18	26       ushort
		CV_16S	3	11	19	27       short
		CV_32S	4	12	20	28       int
		CV_32F	5	13	21	29       float
		CV_64F	6	14	22	30       double */
		using cs = ColorStr;
		using pt = pair<int, int>;

		pt x_norm_range = normRange(x_range, img.cols);
		pt y_norm_range = normRange(y_range, img.rows);
		pt c_norm_range = normRange(c_range, img.channels());
		int xstart = x_norm_range.first, xend = x_norm_range.second;
		int ystart = y_norm_range.first, yend = y_norm_range.second;
		int cstart = c_norm_range.first, cend = c_norm_range.second;

		if (img.type() == 0)
			_printCVMat<uchar>(img, xstart, xend, ystart, yend, true);
		else if (img.type() == 4)
			_printCVMat<int>(img, xstart, xend, ystart, yend, true);
		else if (img.type() == 5)
			_printCVMat<float>(img, xstart, xend, ystart, yend, false);

		else if (img.type() == 16)
			_printCVMat<cv::Vec3b>(img, xstart, xend, ystart, yend, cstart, cend, true);
		else if (img.type() == 20)
			_printCVMat<cv::Vec3i>(img, xstart, xend, ystart, yend, cstart, cend, true);
		else if (img.type() == 21)
			_printCVMat<cv::Vec3f>(img, xstart, xend, ystart, yend, cstart, cend, false);
		else if (img.type() == 22)
			_printCVMat<cv::Vec3d>(img, xstart, xend, ystart, yend, cstart, cend, false);
		else cout << cs::yellow(__func__, ":") << " 该图像的cv::Mat::type()暂不支持，已跳过输出!" << endl;
	}


	template<class MatT>
	void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, bool isInt)
	{
		using cs = ColorStr;
		cout << "cv::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				cout << "[";
			else cout << string(8, ' ') << "[";
			auto* ptr = img.ptr<MatT>(y);
			for (int x = xstart; x < xend; x++)
			{
				if (isInt)
				{
					cout << zfillInt(ptr[x], 3, ' ');
					if (x != xend - 1)
						cout << " ";
				}
				else
				{
					cout << zfillFlt(ptr[x], 3, 4, 4, ' ', '0');
					if (x != xend - 1)
						cout << " ";
				}
			}
			if (y != yend - 1)
				cout << "]" << endl;
			else cout << "]";
		}
		cout << "}";
	}

	template<class cvVec>
	void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, int cstart, int cend, bool isInt)
	{
		using cs = ColorStr;
		cout << "cv::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				cout << "[";
			else cout << string(8, ' ') << "[";
			auto* ptr = img.ptr<cvVec>(y);
			for (int x = xstart; x < xend; x++)
			{
				cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					if (isInt)
					{
						cout << zfillInt(ptr[x][c], 5, ' ');
						if (c != cend - 1)
							cout << " ";
					}
					else
					{
						cout << zfillFlt(ptr[x][c], 3, 4, 4, ' ', '0');
						if (c != cend - 1)
							cout << " ";
					}
				}
				if (x != xend - 1)
					cout << ") ";
				else cout << ")";
			}
			if (y != yend - 1)
				cout << "]" << endl;
			else cout << "]";
		}
		cout << "}";
	}


	void _print(const cv::Mat& img)  //可能是opencv源码using了cv::print导致print(cv::Mat)被劫持
	{
		cout << endl;
		printMat(img);
	}


	template<class T>
	void _print(const cv::Point_<T>& pt)
	{
		cout << "(" << pt.x << ", " << pt.y << ")";
	}

	template<class T>
	void _print(const cv::Point3_<T>& pt)
	{
		cout << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
	}

	template<class T>
	void _print(const cv::Size_<T>& sz)
	{
		cout << "(" << sz.width << ", " << sz.height << ")";
	}

	void _print(const cv::MatSize& sz)
	{
		cout << "(" << sz << ")";
	}

	template<class T>
	void _print(const cv::Rect_<T>& rect)
	{
		cout << "(" << rect.x << " "
			<< rect.y << " "
			<< rect.width << " "
			<< rect.height << ")";
	}
}
