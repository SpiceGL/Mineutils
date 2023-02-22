#pragma once
#include<string>
#include"opencv2/opencv.hpp"
#include"rknn_api.h"

using std::string;
namespace rknn 
{
	typedef struct 
	{
		int w;
		int h;
		int c;
		float* data = nullptr;
	} Tensor;


	class Net 
	{
	public:
		rknn_context ctx;
		Net() = default;
		~Net()
		{
			clear();
		}
		//Net(const Net& _net) = delete;
		//Net& operator=(const Net& _net) = delete;

		void clear()
		{
			if (ctx >= 0) {
				rknn_destroy(ctx);
			}
		}

		int loadModel(string filename)
		{
			FILE* fp = fopen(filename.c_str(), "rb");
			if (fp == nullptr) {
				printf("fopen %s fail!\n", filename);
				return -1;
			}
			fseek(fp, 0, SEEK_END);
			size_t model_len = ftell(fp);
			unsigned char* model = (unsigned char*)malloc(model_len);
			fseek(fp, 0, SEEK_SET);
			if (model_len != fread(model, 1, model_len, fp)) {
				printf("fread %s fail!\n", filename);
				free(model);
				model = nullptr;
				return -2;
			}
			if (fp) {
				fclose(fp);
			}
			int ret = rknn_init(&ctx, model, model_len, 0);
			if (ret < 0) {
				free(model);
				model = nullptr;
				printf("rknn_init fail! ret=%d\n", ret);
				return -3;
			}
			free(model);
			model = nullptr;
			return 0;
		}
	};


	class Extractor {
	private:
		const Net* net;
		rknn_input_output_num io_num;
		rknn_output* outputs;
		rknn_tensor_attr* output_attr;

		template<size_t N>
		int inference(rknn_input(&inputs)[N])
		{
			int ret = rknn_inputs_set(net->ctx, io_num.n_input, inputs);
			if (ret < 0) {
				printf("rknn_input_set fail! ret=%d\n", ret);
				return -3;
			}
			//else print("rknn_input_set successed!!");

			ret = rknn_run(net->ctx, nullptr);
			if (ret < 0) {
				printf("rknn_run fail! ret=%d\n", ret);
				return -4;
			}
			//else print("rknn_run sucessed!!");

			ret = rknn_outputs_get(net->ctx, io_num.n_output, outputs, NULL);
			if (ret < 0) {
				printf("rknn_outputs_get fail! ret=%d\n", ret);
				return -5;
			}
			return 0;
		}

	public:
		Extractor(const Extractor& _temp) = delete;
		Extractor& operator=(const Extractor& _temp) = delete;

		Extractor() { }

		Extractor(const Net* _net)
		{
			this->net = _net;
			int ret = rknn_query(net->ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
			if (ret != RKNN_SUCC) {
				printf("rknn_query fail! ret=%d\n", ret);
				return;
			}

			int outputs_size = sizeof(rknn_output) * io_num.n_output;   //需要变量中转否则可能内存越界
			outputs = (rknn_output*)malloc(outputs_size);
			memset(outputs, 0, outputs_size);

			for (size_t i = 0; i < io_num.n_output; i++)
				outputs[i].want_float = 1;

			int output_attr_size = sizeof(rknn_tensor_attr) * io_num.n_output;
			output_attr = (rknn_tensor_attr*)malloc(output_attr_size);
			memset(output_attr, 0, output_attr_size);

			for (size_t i = 0; i < io_num.n_output; i++) {
				output_attr[i].index = i;
				ret = rknn_query(net->ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attr[i]),
					sizeof(rknn_tensor_attr));
				if (ret != RKNN_SUCC) {
					printf("rknn_query fail! ret=%d\n", ret);
				}
			}
		}

		~Extractor()
		{
			rknn_outputs_release(net->ctx, io_num.n_output, outputs);
			if (outputs != nullptr) {
				free(outputs);
				outputs = nullptr;
			}
			if (output_attr != nullptr) {
				free(output_attr);
				output_attr = nullptr;
			}
		}

		/*无输入创建对象后，使用此函数加载网络*/
		void loadNet(const Net* _net)
		{
			this->net = _net;
			int ret = rknn_query(net->ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
			if (ret != RKNN_SUCC) {
				printf("rknn_query fail! ret=%d\n", ret);
				return;
			}

			int outputs_size = sizeof(rknn_output) * io_num.n_output;   //需要变量中转否则可能内存越界
			outputs = (rknn_output*)malloc(outputs_size);
			memset(outputs, 0, outputs_size);

			for (size_t i = 0; i < io_num.n_output; i++)
				outputs[i].want_float = 1;

			int output_attr_size = sizeof(rknn_tensor_attr) * io_num.n_output;
			output_attr = (rknn_tensor_attr*)malloc(output_attr_size);
			memset(output_attr, 0, output_attr_size);

			for (size_t i = 0; i < io_num.n_output; i++) {
				output_attr[i].index = i;
				ret = rknn_query(net->ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attr[i]),
					sizeof(rknn_tensor_attr));
				if (ret != RKNN_SUCC) {
					printf("rknn_query fail! ret=%d\n", ret);
				}
			}
		}

		/*单输入网络*/
		int input(const cv::Mat& in)
		{
			if (in.empty()) {
				return -1;
			}
			rknn_input inputs[1];
			memset(inputs, 0, sizeof(inputs));
			inputs[0].index = 0;
			inputs[0].buf = in.data;
			inputs[0].size = in.cols * in.rows * in.channels();

			inputs[0].pass_through = 0;   //若为1，则后两个设置无效?
			inputs[0].type = RKNN_TENSOR_UINT8;
			inputs[0].fmt = RKNN_TENSOR_NHWC;

			this->inference(inputs);

			return 0;
		}

		/*多输入网络*/
		template<size_t N>
		int inputs(const cv::Mat (&ins)[N])
		{
			/*if (ins.size() == 0) {
				return -1;
			}*/
			rknn_input inputs[N];
			for (int i = 0; i < N; ++i)
			{
				memset(inputs, 0, sizeof(inputs));
				inputs[i].index = 0;
				inputs[i].buf = ins[i].data;
				inputs[i].size = ins[i].cols * ins[i].rows * ins[i].channels();

				inputs[i].pass_through = 0;   //若为1，则后两个设置无效，网络不对输入图像进行任何前处理
				inputs[i].type = RKNN_TENSOR_UINT8;
				inputs[i].fmt = RKNN_TENSOR_NHWC;
			}

			this->inference(inputs);

			return 0;
		}

		/*根据输出层序号获取输出，序号从0开始*/
		int extract(int out_id, Tensor& out_tensor)
		{
			/*buf存储格式应该是：anchor_type_num * grid_num * box_data_size
			[[x, y, w, h, box_conf, cls_conf1, ...], [x, y, w, h, box_conf, cls_conf1, ...], ...
			[[x, y, w, h, box_conf, cls_conf1, ...], [x, y, w, h, box_conf, cls_conf1, ...], ...
			[[x, y, w, h, box_conf, cls_conf1, ...], [x, y, w, h, box_conf, cls_conf1, ...], ...]
			*/
			out_tensor.h = output_attr[out_id].dims[2];
			out_tensor.w = output_attr[out_id].dims[1];
			out_tensor.c = output_attr[out_id].dims[0];   //似乎有点误导，dims里是反着写的

			out_tensor.data = (float*)outputs[out_id].buf;
			return 0;
		}

		/*每一次获取输出完毕都需要调用一次*/
		void releaseOutputs()
		{
			rknn_outputs_release(net->ctx, io_num.n_output, outputs);
		}
	};

}
