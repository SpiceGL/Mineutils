#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<string>
#include<vector>
#include"calibrate_cam.h"
#include"mineutilshpp/__mineutils__.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace cv;
using namespace mineutils;

#define COEFFS_NEWCAMERAMATRIX 1.0


CamCalibrator::CamCalibrator(cv::Size _imsize)
{
	this->imsize = _imsize;
	loadParam();
}


void CamCalibrator::loadParam()
{
	FileStorage param_file(param_path, FileStorage::READ);
	param_file["camera_matrix"] >> intrinsic_matrix;
	param_file["distortion"] >> distortion_coeffs;
	param_file["mapx"] >> mapx;
	param_file["mapy"] >> mapy;

	param_file.release();

	if (intrinsic_matrix.empty() or distortion_coeffs.empty() or mapx.empty() or mapy.empty())
	{
		printf("!Warning! CamCalibrator::loadParam(): 相机畸变系数等参数不存在，正在重新标定......\n");
		this->normalCalibrate();
	}
}


int CamCalibrator::normalCalibrate()
{
	return this->normalCalibrate(this->chess_dir, false);
}


int CamCalibrator::normalCalibrate(string _chess_dir, bool if_show_res)
{
	int block_w = 9;
	int block_h = 6;

	int block_size = block_w * block_h;

	vector<Point3f> temp_ArrayPoint3f(block_size);
	vector<Point2f> temp_ArrayPoint2f(block_size);

	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;

	std::vector<cv::Vec3d> rotation_vectors;    /* 每幅圖像的旋轉向量 */
	std::vector<cv::Vec3d> translation_vectors;    /* 每幅圖像的平移向量 */
	//Mat rotation_vectors;
	//Mat translation_vectors;
	vector<Point2f> CornerArray;

	Mat Mat_Image_Frame;

	char Temp_String[100] = "";

	/*FILE *F_out;
	F_out = fopen("Point\\point.txt", "w");*/

	int NowCount = 0;

	//sprintf(Temp_String, "InputImages\\%s\\000.%s", INPUTCLASS, INPUTEXT);
	//Mat showAreaImage = imread(Temp_String);

	vector<string> impaths = Path::listDir(_chess_dir);
	if (impaths.size() == 0)
	{
		print(fstr("!!!Error!!! CamCalibrator::{}: 请检查目录{}下是否存在棋盘格图像！", __func__, _chess_dir));
		exit(0);
	}
	Mat showAreaImage = imread(impaths[0]);
	if (showAreaImage.empty())
	{
		print(fstr("Read Image {} failed!", impaths[0]));
		return -1;
	}
	if ((showAreaImage.rows != imsize.width) or (showAreaImage.cols != imsize.height))
		cv::resize(showAreaImage, showAreaImage, imsize);

	Size img_size = showAreaImage.size();

	cout << "img_size: " << img_size << endl;

	for (string impath : impaths)
	{
		Mat_Image_Frame = imread(impath, 1);
		if (Mat_Image_Frame.empty()) 
		{
			print(fstr("Read Image {} failed!", impath));
			continue;
		}
		if ((Mat_Image_Frame.rows != imsize.width) or (Mat_Image_Frame.cols != imsize.height))
			cv::resize(Mat_Image_Frame, Mat_Image_Frame, imsize);
		Mat gray;
		cvtColor(Mat_Image_Frame, gray, CV_BGR2GRAY);
		int found = findChessboardCorners(gray, Size(block_w, block_h), CornerArray, CV_CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		/*fout << "角点座標：" << endl;
		fout << CornerArray << endl;*/

		if (found) {

			cv::TermCriteria criteria = cv::TermCriteria(
				cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
				40,
				0.1);

			//亚像素检测
			cv::cornerSubPix(gray, CornerArray, cv::Size(5, 5), cv::Size(-1, -1), criteria);

			for (int i = 0; i < CornerArray.size(); i++) {

				temp_ArrayPoint2f[i].x = CornerArray[i].x;
				temp_ArrayPoint2f[i].y = CornerArray[i].y;

				temp_ArrayPoint3f[i].x = float(i / block_w);   //可能是x代表行，y代表列
				temp_ArrayPoint3f[i].y = float(i % block_w);

				// temp_ArrayPoint3f[i].x = float(i % block_w);
				// temp_ArrayPoint3f[i].y = float(i / block_w);

				temp_ArrayPoint3f[i].z = float(0);


				/*fprintf(F_out, "%f,%f,%f,%f,%f\n",	temp_ArrayPoint2f[i].x,
					temp_ArrayPoint2f[i].y,
					temp_ArrayPoint3f[i].x,
					temp_ArrayPoint3f[i].y,
					temp_ArrayPoint3f[i].z
					);*/


					//Draw and show the images
				if (if_show_res)
					drawChessboardCorners(Mat_Image_Frame, Size(block_w, block_h), CornerArray, found);
				//Save the Drawlined images.
				//sprintf(Temp_String, "InputImages\\%s\\Point\\%03d.%s", INPUTCLASS,NowCount,INPUTEXT);
				//imwrite(Temp_String, Mat_Image_Frame);
			}

			image_points.push_back(temp_ArrayPoint2f);
			object_points.push_back(temp_ArrayPoint3f);

			//cout << image_points[0] << endl;
			//cout << object_points[0] << endl;

			if (if_show_res)
			{
				cv::imshow("PointFrame", Mat_Image_Frame);
				cv::waitKey(1000);
			}
		}
	}

	//Save the Area images.
	if (temp_ArrayPoint2f.size() < 1)
	{
		print(fstr("!!!Error!!! CamCalibrator::{}: 没有可用的棋盘格图像，请重新拍摄标定图像！", __func__));
		exit(0);
	}


	//Calibration setup the flag
	int flag = 0;
	cv::TermCriteria critia(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 200000, 1);

	cout << "Calibration Start" << endl;
	double rms = cv::calibrateCamera(object_points, image_points, img_size, intrinsic_matrix, distortion_coeffs, rotation_vectors, translation_vectors, flag, cv::TermCriteria(3, 20, 1e-6));
	cout << "rms scole=" << rms << endl;


	cout << "开始评价定标结果………………" << endl;
	double total_err = 0.0;                   /* 所有图像的平均误差的总和 */
	double err = 0.0;                        /* 每幅图像的平均误差 */
	vector<Point2f>  image_points2;             /****   保存重新计算得到的投影点    ****/

	cout << "每幅图像的定标误差：" << endl;
	cout << "每幅图像的定标误差：" << endl << endl;

	for (int i = 0; i < object_points.size(); i++)
	{
		vector<Point3f> tempPointSet = object_points[i];
		/****    通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点     ****/
		cv::projectPoints(tempPointSet, rotation_vectors[i], translation_vectors[i], intrinsic_matrix, distortion_coeffs, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = image_points[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (size_t j = 0; j != tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= block_size;
		cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;

	}
	cout << "总体平均误差：" << total_err / object_points.size() << "像素" << endl;
	cout << "评价完成！" << endl;


	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0));


	// ==================== Save Data ==================== 

	cv::Mat Knew;
	Knew.at<double>(0, 0) *= COEFFS_NEWCAMERAMATRIX;
	Knew.at<double>(1, 1) *= COEFFS_NEWCAMERAMATRIX;
	Mat R = Mat::eye(3, 3, CV_32F);
	cv::initUndistortRectifyMap(intrinsic_matrix, distortion_coeffs, R, Knew, imsize, CV_16SC2, mapx, mapy);

	FileStorage param_file(param_path, FileStorage::WRITE);
	param_file << "camera_matrix" << intrinsic_matrix;
	param_file << "distortion" << distortion_coeffs;

	param_file << "mapx" << mapx;
	param_file << "mapy" << mapy;

	param_file.release();

	cout << "完成保存" << endl;
	cout << endl;

	return 0;
}


int CamCalibrator::undistortFrame(cv::Mat& img_in, cv::Mat& img_out, bool if_show_res)
{
	Mat Knew;
	cv::Mat Mat_Image_Frame = img_in.clone();
	if ((Mat_Image_Frame.rows != imsize.width) or (Mat_Image_Frame.cols != imsize.height))
		cv::resize(Mat_Image_Frame, Mat_Image_Frame, imsize);

	if (intrinsic_matrix.empty() or distortion_coeffs.empty() or mapx.empty() or mapy.empty())
	{
		this->normalCalibrate();
	}

	intrinsic_matrix.copyTo(Knew);

	//秸俱跌絛瞅
	Knew.at<double>(0, 0) *= COEFFS_NEWCAMERAMATRIX;
	Knew.at<double>(1, 1) *= COEFFS_NEWCAMERAMATRIX;

	cv::undistort(Mat_Image_Frame, img_out, intrinsic_matrix, distortion_coeffs, Knew);

	if (if_show_res)
	{
		cv::Mat concat_img;
		cv::hconcat(Mat_Image_Frame, img_out, concat_img);
		quickShowCV("origin-undistort", concat_img, 0);
	}

	return 0;
}

int CamCalibrator::undistortFrame(string& impath, cv::Mat& img_out, bool if_show_res)
{
	cv::Mat img = cv::imread(impath, 1);
	if (img.empty())
	{
		print(fstr("Read image {} failed!", impath));
		return -1;
	}
	return undistortFrame(img, img_out, if_show_res);
}

int CamCalibrator::remapFrame(cv::Mat& img_in, cv::Mat& img_out, bool if_show_res)
{
	Mat Knew;
	cv::Mat Mat_Image_Frame = img_in.clone();
	if ((Mat_Image_Frame.rows != imsize.width) or (Mat_Image_Frame.cols != imsize.height))
		cv::resize(Mat_Image_Frame, Mat_Image_Frame, imsize);

	if (intrinsic_matrix.empty() or distortion_coeffs.empty() or mapx.empty() or mapy.empty())
	{
		this->normalCalibrate();
	}

	//mapping
	//cv::Mat transformation_x, transformation_y;
	//cv::convertMaps(mapx, mapy, transformation_x, transformation_y, CV_16SC2);
	//remap(Mat_Image_Frame, img_out, transformation_x, transformation_y, CV_INTER_LINEAR);
	//printMat(img_out, { 640,645 }, { 360,363 });
	cv::remap(Mat_Image_Frame, img_out, mapx, mapy, CV_INTER_LINEAR);

	if (if_show_res)
	{
		cv::Mat concat_img;
		cv::hconcat(Mat_Image_Frame, img_out, concat_img);
		quickShowCV("origin-remap", concat_img, 0);
	}

	return 0;
}

int CamCalibrator::remapFrame(string& impath, cv::Mat& img_out, bool if_show_res)
{
	cv::Mat img = cv::imread(impath, 1);
	if (img.empty())
	{
		print(fstr("Read image {} failed!", impath));
		return -1;
	}
	return remapFrame(img, img_out, if_show_res);
}