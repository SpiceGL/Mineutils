#pragma once
#include<string>
#include<vector>
#include<opencv2/opencv.hpp>
#include<limits.h>
#include<stdlib.h>

#include"str.hpp"
#include"exception.hpp"
#include"points.hpp"



namespace mineutils
{
	/*
	#cv2.VideoWriter_fourcc('X', '2', '6', '4'), 该参数是较新的MPEG-4编码,产生的文件较小,文件扩展名应为.mp4
	#cv2.VideoWriter_fourcc('P', 'I', 'M', 'I'), 该参数是较旧的MPEG-1编码,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('M', 'P', '4', '2'), 该参数是MPEG-2编码,产生的文件不会特别大,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('D', 'I', 'V', '3'), 该参数是MPEG-3编码,产生的文件不会特别大,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('D', 'I', 'V', 'X'), 该参数是MPEG-4编码,产生的文件不会特别大,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('X', 'V', 'I', 'D'), 该参数是较旧的MPEG-4编码,产生的文件不会特别大,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('M', 'P', '4', 'V'), 该参数也是较旧的MPEG-4编码,产生的文件不会特别大,文件扩展名应为.m4v
	#cv2.VideoWriter_fourcc('T', 'H', 'E', 'O'), 该参数是Ogg Vorbis,产生的文件相对较大,文件名后缀为.ogv
	#cv2.VideoWriter_fourcc('F', 'L', 'V', '1'), 该参数是Flash视频,产生的文件相对较大,文件名后缀为.flv
	#cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), 该参数是motion-jpeg编码,产生的文件较大,文件名后缀为.avi
	#cv2.VideoWriter_fourcc('I', '4', '2', '0'),该参数是未压缩的YUV编码,4:2:0色度子采样,这种编码广泛兼容,但会产生特别大的文件,文件扩展名应为.avi ```
	*/
	/*------------------------------------声明-------------------------------------*/

	std::string setWindowCV(const std::string& win_name, cv::Size size = { -1, -1 },
		std::pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	int loopShowCV(const std::string& win_name, cv::Mat& img, float wait = 1);

	void quickShowCV(const std::string& win_name, cv::Mat& img,
		float wait = 1, bool close = false, cv::Size size = { -1, -1 },
		std::pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void quickPlayCV(const std::string& win_name, const std::string& video_path,
		float wait = 30, cv::Size size = { -1, -1 },
		std::pair<int, int> position = { -1, -1 }, int flag = cv::WINDOW_FREERATIO);

	void putLabelCV(cv::Mat& img, const std::string& label, cv::Point position, cv::Scalar text_color = { 255,255,255 },
		int word_type = cv::FONT_HERSHEY_SIMPLEX, float word_scale = 1, int text_thickness = 2,
		bool have_bg = true, cv::Scalar bg_color = {255, 0, 0});

	void putBoxCV(cv::Mat& img, const LTRB& ltrb, const std::string& label = "",
		cv::Scalar bbox_color = { 0,255,0 }, cv::Scalar text_color = { 255,255,255 },
		int word_type = cv::FONT_HERSHEY_SIMPLEX, float word_scale = 1, 
		int bbox_thickness = 3, int text_thickness = 2);

	void putBoxCV(cv::Mat& img, XYWH xywh, const std::string& label = "",
		cv::Scalar bbox_color = { 0,255,0 }, cv::Scalar text_color = { 255,255,255 },
		int word_type = cv::FONT_HERSHEY_SIMPLEX, float word_scale = 1, 
		int bbox_thickness = 3, int text_thickness = 2);

	template<class T=int>
	void channelInit(cv::Mat& mat, cv::Point3_<T> channel_value = {0, 0, 0});

	template<class Tx = std::pair<int, int>, class Ty = std::pair<int, int>, class Tc = std::pair<int, int>>
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
	
	
	//快速设置窗口属性
	inline std::string setWindowCV(const std::string& win_name, cv::Size size, std::pair<int, int> position, int flag)
	{
		cv::namedWindow(win_name, flag);
		if (size.width != -1)
			cv::resizeWindow(win_name, size);
		if (position.first != -1)
			cv::moveWindow(win_name, position.first, position.second);
		return win_name;
	}

	//快速显示图像，窗口属性在函数外设置，推荐用于循环体中，返回键入的key值
	inline int loopShowCV(const std::string& win_name, cv::Mat& img, float wait)
	{
		cv::imshow(win_name, img);
		int k = cv::waitKey(wait) & 0xff;
		//bool go_on;
		//if (k == 27 or k == int('q'))
		//	go_on = false;
		//else go_on = true;
		return k;
	}


	//快速显示图像，一步到位设置窗口和显示属性
	inline void quickShowCV(const std::string& win_name, cv::Mat& img, float wait, bool close,
		cv::Size size, std::pair<int, int> position, int flag)
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

	//快速显示视频
	inline void quickPlayCV(const std::string& win_name, const std::string& video_path, float wait,
		cv::Size size, std::pair<int, int> position, int flag)
	{
		using cs = ColorStr;
		auto cap = cv::VideoCapture(video_path);
		if (not cap.isOpened())
		{
			std::cout << makeMessageE(__func__, "视频文件打开失败，已跳过播放！") << std::endl;
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
				std::cout << makeMessageN(__func__, "视频已播放结束。") << std::endl;
				break;
			}
			bool go_on = loopShowCV(win_name, frame, wait);
			if (not go_on)
			{
				cv::destroyWindow(win_name);
				std::cout << makeMessageN(__func__, "收到中止信号，提前结束视频播放！")  << std::endl;
				break;
			}
		}
		cap.release();
	}


	/*---------------------------------------------------------------------------------*/
	//为图像添加文字
	inline void putLabelCV(cv::Mat& img, const std::string& label, cv::Point position, cv::Scalar text_color,
		int word_type, float word_scale, int text_thickness, bool have_bg, cv::Scalar bg_color)
	{
		/*	position：文字左下角位置 */
		if (label.size() != 0)
		{
			cv::Point& c1 = position;
			if (have_bg)
			{
				int baseline;
				cv::Size text_size = cv::getTextSize(label, word_type, word_scale, text_thickness, &baseline);
				cv::Point c2 = { c1.x + text_size.width, c1.y - (int)(text_size.height*1.2) };
				cv::rectangle(img, c1, c2, bg_color, -1);
			}
			cv::putText(img, label, c1, word_type, word_scale, text_color, text_thickness, cv::LINE_AA);
		}
	}

	//为图像添加检测框及标签
	inline void putBoxCV(cv::Mat& img, const LTRB& ltrb, const std::string& label,
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

	inline void putBoxCV(cv::Mat& img, XYWH xywh, const std::string& label,
		cv::Scalar bbox_color, cv::Scalar text_color, int word_type, float word_scale,
		int bbox_thickness, int text_thickness)
	{
		putBoxCV(img, xywh.toLTRB(), label,
			bbox_color, text_color, word_type, word_scale,
			bbox_thickness, text_thickness);
	}

	/*---------------------------------------------------------------------------------*/

	//自定义3个通道的值
	template<class T>
	inline void channelInit(cv::Mat& mat, cv::Point3_<T> channel_value)
	{
		std::vector<cv::Mat> ma_mb_mc;
		cv::split(mat, ma_mb_mc);
		ma_mb_mc[0] = channel_value.x;
		ma_mb_mc[1] = channel_value.y;
		ma_mb_mc[2] = channel_value.z;
		cv::merge(ma_mb_mc, mat);
	}


	//显示cv::Mat的值
	template<class Tx, class Ty, class Tc>
	inline void printMat(const cv::Mat& img, Tx x_range, Ty y_range, Tc c_range)
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
		using pt = std::pair<int, int>;

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
		else std::cout << makeMessageW(__func__, "该图像的cv::Mat::type()暂不支持，已跳过输出！")  << std::endl;
	}


	template<class MatT>
	inline void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, bool isInt)
	{
		using cs = ColorStr;
		std::cout << "cv::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				std::cout << "[";
			else std::cout << std::string(8, ' ') << "[";
			auto* ptr = img.ptr<MatT>(y);
			for (int x = xstart; x < xend; x++)
			{
				if (isInt)
				{
					std::cout << zfillInt(ptr[x], 3, ' ');
					if (x != xend - 1)
						std::cout << " ";
				}
				else
				{
					std::cout << zfillFlt(ptr[x], 3, 4, ' ', '0');
					if (x != xend - 1)
						std::cout << " ";
				}
			}
			if (y != yend - 1)
				std::cout << "]" << std::endl;
			else std::cout << "]";
		}
		std::cout << "}\n";
	}

	template<class cvVec>
	inline void _printCVMat(const cv::Mat& img, int xstart, int xend, int ystart, int yend, int cstart, int cend, bool isInt)
	{
		using cs = ColorStr;
		std::cout << "cv::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				std::cout << "[";
			else std::cout << std::string(8, ' ') << "[";
			auto* ptr = img.ptr<cvVec>(y);
			for (int x = xstart; x < xend; x++)
			{
				std::cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					if (isInt)
					{
						std::cout << zfillInt(ptr[x][c], 5, ' ');
						if (c != cend - 1)
							std::cout << " ";
					}
					else
					{
						std::cout << zfillFlt(ptr[x][c], 3, 4, ' ', '0');
						if (c != cend - 1)
							std::cout << " ";
					}
				}
				if (x != xend - 1)
					std::cout << ") ";
				else std::cout << ")";
			}
			if (y != yend - 1)
				std::cout << "]" << std::endl;
			else std::cout << "]";
		}
		std::cout << "}\n";
	}

	//可能是opencv源码using了cv::print导致print(cv::Mat)被劫持
	inline void _print(const cv::Mat& img)  
	{
		std::cout << std::endl;
		printMat(img);
	}


	template<class T>
	inline void _print(const cv::Point_<T>& pt)
	{
		std::cout << "(" << pt.x << ", " << pt.y << ")";
	}

	template<class T>
	inline void _print(const cv::Point3_<T>& pt)
	{
		std::cout << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
	}

	template<class T>
	inline void _print(const cv::Size_<T>& sz)
	{
		std::cout << "(" << sz.width << ", " << sz.height << ")";
	}

	inline void _print(const cv::MatSize& sz)
	{
		std::cout << "(" << sz << ")";
	}

	template<class T>
	inline void _print(const cv::Rect_<T>& rect)
	{
		std::cout << "(" << rect.x << " "
			<< rect.y << " "
			<< rect.width << " "
			<< rect.height << ")";
	}
}
