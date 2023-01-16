#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<limits.h>
#include<stdlib.h>

#include"ncnn/net.h"

#include"str.hpp"
#include"exception.hpp"
#include"index.hpp"


namespace mineutils
{
	/*----------------------------------声明--------------------------------------*/
	bool loadNcnn(ncnn::Net& net_out, const std::string& param_path, const std::string& bin_path);
	
	std::vector<ncnn::Mat> quickRunNcnn(ncnn::Net& net_in, ncnn::Mat& input, const std::string& in_name, const std::vector<std::string>& out_names);

	std::vector<ncnn::Mat> quickRunNcnn(const std::string& param_path, const std::string& model_path, ncnn::Mat& input, const std::string& in_name, const std::vector<std::string>& out_names);

	template<class Tx = std::pair<int, int>, class Ty = std::pair<int, int>, class Tc = std::pair<int, int>>
	void printMat(const ncnn::Mat& m, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });



	/*----------------------------------定义--------------------------------------*/
	inline bool loadNcnn(ncnn::Net& net_out, const std::string& param_path, const std::string& bin_path)
	{
		//ncnn::Net net;
		if (net_out.load_param(param_path.c_str()))
		{
			std::cout << makeMessageW(__func__, fstr("加载param文件{}失败！ ", param_path)) << std::endl;
			return false;
		}
		if (net_out.load_model(bin_path.c_str()))
		{
			std::cout << makeMessageW(__func__, fstr("加载bin文件{}失败！ ", bin_path)) << std::endl;
			return false;
		}
		return true;
	}
	
	inline std::vector<ncnn::Mat> quickRunNcnn(ncnn::Net& net_in, ncnn::Mat& input, const std::string& in_name, 
		const std::vector<std::string>& out_names)
	{
		
		ncnn::Extractor extractor = net_in.create_extractor();
		extractor.input(in_name.c_str(), input);

		std::vector<ncnn::Mat> outs;
		ncnn::Mat out;
		for (const std::string& out_id : out_names)
		{
			extractor.extract(out_id.c_str(), out);
			outs.push_back(out);
		}
		return outs;
	}

	inline std::vector<ncnn::Mat> quickRunNcnn(const std::string& param_path, const std::string& model_path, ncnn::Mat& input, const std::string& in_name, const std::vector<std::string>& out_names)
	{
		ncnn::Net net;
		net.load_param(param_path.c_str());
		net.load_model(model_path.c_str());
		return quickRunNcnn(net, input, in_name, out_names);
	}


	template<class Tx, class Ty, class Tc>
	inline void printMat(const ncnn::Mat& m, Tx x_range, Ty y_range, Tc c_range)   //输出ncnn的Mat，只支持三维图像Mat
	{
		using Range = std::pair<int, int>;
		Range x_norm_range = normRange(x_range, m.w);
		Range y_norm_range = normRange(y_range, m.h);
		Range c_norm_range = normRange(c_range, m.c);
		int xstart = x_norm_range.first, xend = x_norm_range.second;
		int ystart = y_norm_range.first, yend = y_norm_range.second;
		int cstart = c_norm_range.first, cend = c_norm_range.second;

		const float* ptr = nullptr;
		int bias = ystart * m.w;
		std::cout << "ncnn::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				std::cout << "[";
			else std::cout << std::string(10, ' ') << "[";
			for (int x = xstart; x < xend; x++)
			{
				std::cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					ptr = m.channel(c);
					if (c != cend - 1)
						std::cout <<  ptr[x + bias] << " ";
					else std::cout <<  ptr[x + bias];
				}
				if (x != xend - 1)
					std::cout << ") ";
				else std::cout << ")";
			}
			bias += m.w;
			if (y != yend - 1)
				std::cout << "]" << std::endl;
			else std::cout << "]";
		}
		std::cout << "}\n";
	}

	//std::ostream& operator<<(std::ostream& std::cout, ncnn::Mat& mat)
	//{
	//	printMat(mat);
	//	return std::cout;
	//}

	inline void printMats(const std::vector<ncnn::Mat>& mats)
	{
		std::cout << "Mats{" << std::endl;
		for (auto mat : mats)
			printMat(mat);
		std::cout << "}\n";
	}

	inline void _print(const ncnn::Mat& m)
	{
		printMat(m);
	}

	inline void _print(const std::vector<ncnn::Mat>& mats)
	{
		printMats(mats);
	}
}
