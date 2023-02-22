#pragma once
#include<string>
#include<opencv2/opencv.hpp>
#include"calibrate_cam.h"


class CoorTransformer
{
private:
	std::string param_path = "./cfg/wrap_params.xml";   
	std::string chess_path = "./cfg/chess.jpg";
	std::string world_points_path = "./cfg/points.txt";   //【重要】角点顺序是从右下到左上！
	int chess_width = 3, chess_height = 3;

	cv::Size imsize;
	int undistort_type;   //0 不去畸变   1 undistort函数去畸变   2 remap函数去畸变
	cv::Mat warp_mat, warp_mat_undistort, warp_mat_remap;

	std::vector<cv::Point2f> readWorldPoints(std::vector<cv::Point2f> world_point);
	void loadWarpMat();
	void saveWarpMat();
	cv::Mat getAffineMatrix(std::vector<cv::Point2f> points_camera);

public:
	CoorTransformer(cv::Size _imsize);   
	CoorTransformer(CoorTransformer& temp) = delete;
	CoorTransformer& operator=(CoorTransformer& temp) = delete;

	int resetWarpMat(bool if_show_res = false);

	//_undistort_type:   0 不去畸变  1 undistort函数去畸变  2 remap函数去畸变
	cv::Point2f toWorldCoordinate(cv::Point2f cam_point, int undistort_type);
};
