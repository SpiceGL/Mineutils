#pragma once
#include<string>
#include<opencv2/opencv.hpp>


class CamCalibrator
{
private:
	std::string param_path = "./cfg/calibrate_params.xml";
	std::string chess_dir = "./chess_for_calibrate";
	
	cv::Size imsize;
	cv::Mat intrinsic_matrix, distortion_coeffs;
	cv::Mat mapx, mapy;

	void loadParam();
	
public:
	CamCalibrator(cv::Size _imsize);
	CamCalibrator(const CamCalibrator& _temp) = delete;
	CamCalibrator& operator=(const CamCalibrator& _temp) = delete;
	
	int normalCalibrate(std::string _chess_dir, bool if_show_res=false);
	int normalCalibrate();
	int undistortFrame(cv::Mat& img_in, cv::Mat& img_out, bool if_show_res = false);
	int undistortFrame(std::string& impath_in, cv::Mat& img_out, bool if_show_res = false);
	int remapFrame(cv::Mat& img_in, cv::Mat& img_out, bool if_show_res = false);
	int remapFrame(std::string& impath_in, cv::Mat& img_out, bool if_show_res = false);
};
