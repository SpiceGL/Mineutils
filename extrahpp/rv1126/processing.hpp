#pragma once
#include<vector>
#include<cmath>
#include<opencv2/opencv.hpp>
#include"rknn_api.hpp"
#include"mineutilshpp/__stdutils__.h"

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using namespace mineutils;

int PADDING_TOP, PADDING_LEFT;   //记录padding带来的坐标偏移
float RESIZE_SCALE;   //记录resize带来的尺寸变化比例
cv::Size2f UNPADDING_SHAPE;   //记录未padding时的输入图像尺寸


void letterbox(cv::Mat& img_in, cv::Mat& img_out, bool autopad = true, cv::Size2f shape_in = { 640, 640 }, cv::Scalar color = { 114,114,114 }, int max_stride = 32)
{
	cv::Mat img = img_in.clone();
	//cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	cv::Size2f shape_img = { (float)img.cols, (float)img.rows };
	float fscale = MIN(shape_in.width / shape_img.width, shape_in.height / shape_img.height);
	RESIZE_SCALE = fscale;
	cv::Size unpad_size = { (int)round(shape_img.width * fscale), (int)round(shape_img.height * fscale) };
	UNPADDING_SHAPE = unpad_size;
	float dw, dh;   //短边需要padding的长度
	dw = shape_in.width - unpad_size.width;
	dh = shape_in.height - unpad_size.height;
	if (autopad)
	{
		dw = (int)dw % max_stride;
		dh = (int)dh % max_stride;
	}
	dw /= 2;
	dh /= 2;
	cv::resize(img, img, unpad_size);

	int top = round(dh - 0.1);
	int bottom = round(dh + 0.1);
	int left = round(dw - 0.1);
	int right = round(dw + 0.1);
	PADDING_TOP = top;
	PADDING_LEFT = left;
	cv::copyMakeBorder(img, img_out, top, bottom, left, right, cv::BORDER_CONSTANT, color);
	//img_out.convertTo(img_out, CV_32FC3, 1. / 255.);
}


bool _compileOf_boxes_info(vector<float>& box_info1, vector<float>& box_info2)
{
	return box_info1[4] > box_info2[4];  //降序
}


void rescale(LTRBf& ltrb)
{
	/*将padding带来的坐标变化还原*/
	ltrb.left = MAX(ltrb[0] - PADDING_LEFT, 0);
	ltrb.top = MAX(ltrb[1] - PADDING_TOP, 0);
	ltrb.right = MIN(ltrb[2] - PADDING_LEFT, UNPADDING_SHAPE.width);
	ltrb.bottom = MIN(ltrb[3] - PADDING_TOP, UNPADDING_SHAPE.height);
	/*处理resize带来的坐标变化*/
	ltrb.left /= RESIZE_SCALE;
	ltrb.top /= RESIZE_SCALE;
	ltrb.right /= RESIZE_SCALE;
	ltrb.bottom /= RESIZE_SCALE;
}


class YoloBoxMaker
{
private:
	float sigmoid(float& x)
	{
		return 1. / (1. + exp(-x));
	}

public:
	YoloBoxMaker() { }
	YoloBoxMaker(YoloBoxMaker& _temp) = delete;

	int net_input_shape[2] = { 640, 640 };
	bool agnostic_nms = false;   //若为true，则nms过程不区分类别；否则对每个类别分别做nms
	float conf_thres = 0.25;
	float iou_thres = 0.45;
	int max_wh = MAX(net_input_shape[0], net_input_shape[1]);
	int max_det = 300;
	pair<int, int> nyx[3] = { {80, 80}, {40, 40}, {20, 20} };
	vector<int> stride = { 8, 16, 32 };
	vector<vector<float>> anchor_grid[3] =
	{
		{{10, 13}, {16, 30}, {33, 23}},
		{{30, 61}, {62, 45}, {59, 119}},
		{{116, 90}, {156, 198}, {373, 326}}
	};



	/*输入未处理的xywh型的网络输出，得到xywh型的处理结果*/
	vector<vector<float>> make(rknn::Tensor(&logits)[3])
	{
		/*	logits[3]: {x, y, w, h, box_conf, cls1_conf, cls2_conf, ...}, shape为(bs, 3, ny*nx, cls_num+5)
		*	ny,nx: 该尺度的logits特征图在y和x方向上对应的长度
		*/
		vector<vector<float>> xywh_infos;   //{x, y, width, height, conf, cls_id}
		vector<cv::Rect2d> rects;   //{x, y, width, height}
		vector<float> scores;   //confidence

		for (int idx = 0; idx < 3; ++idx)
		{
			int& ny = this->nyx[idx].first;
			int& nx = this->nyx[idx].second;

			if (logits[idx].w != ny * nx)
			{
				print(fstr("{}: logits[idx]长度与ny*nx不匹配！"));
				return { {} };
			}

			for (int id_h = 0; id_h < logits[idx].h; ++id_h)
			{
				int bias_h = id_h * logits[idx].w * logits[idx].c;   //h变化带来的指针偏移量
				for (int id_w = 0; id_w < logits[idx].w; ++id_w)
				{
					int gridy = id_w / nx;
					int gridx = id_w % nx;

					int bias = bias_h + id_w * logits[idx].c;   //每个box开头地址对应的指针偏移量
					vector<float> xywh_info;


					float x = (this->sigmoid(logits[idx].data[0 + bias]) * 2 - 0.5 + gridx) * stride[idx];
					float y = (this->sigmoid(logits[idx].data[1 + bias]) * 2 - 0.5 + gridy) * stride[idx];
					float w = std::pow((this->sigmoid(logits[idx].data[2 + bias]) * 2), 2) * anchor_grid[idx][id_h][0];
					float h = std::pow((this->sigmoid(logits[idx].data[3 + bias]) * 2), 2) * anchor_grid[idx][id_h][1];

					/*类型*/
					int cls_id = 0;
					float max_cls_conf = FLT_MIN;
					for (int i = 5; i < logits[idx].c; ++i)
					{
						float cls_conf_i = this->sigmoid(logits[idx].data[i + bias]);
						if (cls_conf_i > max_cls_conf)
						{
							max_cls_conf = cls_conf_i;
							cls_id = i - 5;
						}
					}
					float conf = this->sigmoid(logits[idx].data[4 + bias]) * max_cls_conf;
					scores.push_back(conf);
					rects.push_back({ x, y, w, h });
					xywh_infos.push_back({ x, y, w, h, conf, (float)cls_id });
				}
			}
		}

		vector<int> ids;
		cv::dnn::NMSBoxes(rects, scores, conf_thres, iou_thres, ids);
		vector<vector<float>> outputs;
		for (int j = 0; j < ids.size(); ++j)
			outputs.push_back(xywh_infos[ids[j]]);
		if (outputs.size() > max_det)   //如果剩余框过多，则按除置信度保留max_det个框
		{
			std::sort(outputs.begin(), outputs.end(), _compileOf_boxes_info);
			outputs.resize(max_det);
		}
		return outputs;   //{x, y, width, height, conf, cls_id}
	}
};





