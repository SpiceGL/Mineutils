#pragma warning(disable:4996)
#include"transform_coordinate.h"
#include"mineutilshpp/__cvutils__.h"

using namespace std;
using namespace cv;
using namespace mineutils;


vector<Point2f>Push_points_camera_array(vector<Point2f>points_camera, vector<Point2f> CornerArray)
{
	for (int i = 0; i < CornerArray.size(); i++)
	{
		points_camera[i].x = CornerArray[i].x;
		points_camera[i].y = CornerArray[i].y;
	}
	return points_camera;
}


Point2f transformCoordinate(Mat warpMat, Point2f cam_point)
{

	double A = warpMat.ptr<double>(0)[0];
	double B = warpMat.ptr<double>(0)[1];
	double C = warpMat.ptr<double>(0)[2];
	double D = warpMat.ptr<double>(1)[0];
	double E = warpMat.ptr<double>(1)[1];
	double F = warpMat.ptr<double>(1)[2];
	Point2f world_point;//欲转换目标实际坐标

	//坐标换算
	world_point.x = (A * cam_point.x) + (B * cam_point.y + C);
	world_point.y = (D * cam_point.x) + (E * cam_point.y + F);

	return world_point;
}

vector<Point2f>findChessboardCorners(Mat gray, int block_w,int block_h)
{
	vector<Point2f> CornerArray;

	int block_size = block_w * block_h;
	int found = findChessboardCorners(gray, Size(block_w, block_h), CornerArray, CV_CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
	vector<Point2f>points_camera(block_size);
	if (found)
	{
		cv::TermCriteria criteria = cv::TermCriteria(
			cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 40, 0.1);

		//亚像素检测
		cv::cornerSubPix(gray, CornerArray, cv::Size(5, 5), cv::Size(-1, -1), criteria);
		for (int i = 0; i < CornerArray.size(); i++)
		{
			points_camera[i].x = CornerArray[i].x;
			points_camera[i].y = CornerArray[i].y;
		}
	}

	if (!found)
	{
		print(fstr("Error! {}: Can't find chessboard!", __func__));
		exit(0);
	}
	return points_camera;
}


/*----------------------------------------------------------*/
CoorTransformer::CoorTransformer(cv::Size _imsize)
{
	this->imsize = _imsize;
	this->loadWarpMat();
	//if (this->if_undistorted)
	//	this->calibrator_ptr = new CamCalibrator;
	//else this->calibrator_ptr = nullptr;
}


void CoorTransformer::loadWarpMat()
{
	FileStorage fs;
	fs.open(this->param_path, FileStorage::READ);
	fs["warp_mat"] >> this->warp_mat;
	fs["warp_mat_undistort"] >> this->warp_mat_undistort;
	fs["warp_mat_remap"] >> this->warp_mat_remap;
	fs.release();
	if (warp_mat.empty() or warp_mat_undistort.empty() or warp_mat_remap.empty())
	{
		print(fstr("!Warning! CoorTransformer::{}: warp_mat等坐标变换参数不存在，正在重新计算......"), __func__);
		this->resetWarpMat(false);
	}
}

void CoorTransformer::saveWarpMat()
{
	FileStorage fs_warpMat(this->param_path, FileStorage::WRITE);
	fs_warpMat << "warp_mat" << this->warp_mat;
	fs_warpMat << "warp_mat_undistort" << this->warp_mat_undistort;
	fs_warpMat << "warp_mat_remap" << this->warp_mat_remap;
	fs_warpMat.release();
}

vector<Point2f> CoorTransformer::readWorldPoints(vector<Point2f> world_point)
{
	//======================== Reading Coordinate of Point ========================

	fstream file(this->world_points_path);

	if (!file.good()) {

		print(fstr("!!!Error!!! CoorTransformer::{}: 读取世界坐标点文件{}失败，程序已退出！"), __func__, this->world_points_path);
		exit(0);
	}
	else
	{
		cout << "----- Read Point File OK ! -----\n" << endl;
	}

	//================================Auto_find_point _coordinate===================================//
	float objectx, objecty;
	char split_char = ',';
	for (string line; getline(file, line); )   //read stream line by line
	{
		std::istringstream split(line);
		std::vector<std::string> tokens;
		for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));

		objectx = stof(tokens[0]);
		objecty = stof(tokens[1]);

		world_point.push_back(Point2f(objectx, objecty));
	}
	return world_point;
}

cv::Mat CoorTransformer::getAffineMatrix(vector<Point2f> cam_points)
{

	vector<Point2f>world_points;
	world_points = this->readWorldPoints(world_points);
	if (cam_points.size() != world_points.size())
	{
		print(fstr("!!!Error!!! CoorTransformer::{}: 棋盘格上的点数与世界坐标点数不一样，请检查！程序已退出！"), __func__);
		exit(0);
	}

	Mat _warp_mat;
	_warp_mat = cv::estimateRigidTransform(cam_points, world_points, true);

	//仿射矩阵

	return _warp_mat;
}

int CoorTransformer::resetWarpMat(bool if_show_res)
{
	print("----------------------- start reset warp mat ------------------------");
	Mat chess_img = imread(this->chess_path);
	if (chess_img.empty())
	{
		print(fstr("!!!Error!!! CoorTransformer::{}, 用于计算坐标变换矩阵的棋盘格图像{}不存在，程序已中止！", __func__, this->chess_path));
		exit(0);
	}
	if ((chess_img.rows != imsize.width) or (chess_img.cols != imsize.height))
		cv::resize(chess_img, chess_img, imsize);
	cv::Mat chess_show = chess_img.clone();

	cv::Mat chess_img_gray;
	cv::cvtColor(chess_img, chess_img_gray, CV_BGR2GRAY);
	vector<Point2f>cam_points = findChessboardCorners(chess_img_gray, this->chess_width, this->chess_height);
	cv::drawChessboardCorners(chess_show, { chess_width, chess_height }, cam_points, true);
	if (if_show_res)
		quickShowCV("chess", chess_show, 10000, true);
	warp_mat = this->getAffineMatrix(cam_points);

	CamCalibrator calibrator(imsize);
	cv::Mat chess_img_undis;
	calibrator.undistortFrame(chess_img, chess_img_undis, false);
	chess_show = chess_img_undis.clone();
	cv::cvtColor(chess_img_undis, chess_img_gray, CV_BGR2GRAY);
	cam_points = findChessboardCorners(chess_img_gray, this->chess_width, this->chess_height);
	if (if_show_res)
	{
		cv::drawChessboardCorners(chess_show, { chess_width, chess_height }, cam_points, true);
		quickShowCV("chess_undis", chess_show, 5000, true);
	}
	warp_mat_undistort = this->getAffineMatrix(cam_points);

	cv::Mat chess_img_remap;
	calibrator.remapFrame(chess_img, chess_img_remap, false);
	chess_show = chess_img_remap.clone();
	cv::cvtColor(chess_img_remap, chess_img_gray, CV_BGR2GRAY);
	cam_points = findChessboardCorners(chess_img_gray, this->chess_width, this->chess_height);
	if (if_show_res)
	{
		cv::drawChessboardCorners(chess_show, { chess_width, chess_height }, cam_points, true);
		quickShowCV("chess_remap", chess_show, 5000, true);
	}
	warp_mat_remap = this->getAffineMatrix(cam_points);

	this->saveWarpMat();
	print("---------------- warp mats have been saved! ----------------------");
	return 0;
}



Point2f CoorTransformer::toWorldCoordinate(Point2f cam_point, int undistort_type)
{
	if (warp_mat.empty() or warp_mat_undistort.empty() or warp_mat_remap.empty())
	{
		print(fstr("!Warning! CoorTransformer::{}: warp_mat等坐标变换参数不存在，正在重新计算......"), __func__);
		this->resetWarpMat(false);
	}

	Point2f world_position;
	if (undistort_type == 1)   
		world_position = transformCoordinate(warp_mat_undistort, cam_point);
	else if (undistort_type == 2)
		world_position = transformCoordinate(warp_mat_remap, cam_point);
	else world_position = transformCoordinate(warp_mat, cam_point);
	return world_position;
}