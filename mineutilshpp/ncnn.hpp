#pragma once
#include<iostream>
#include<string>
#include<limits.h>
#include<stdlib.h>

#include"ncnn/net.h"

#include"index.hpp"
#include"str.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;
	using std::pair;
	using std::string;
	using std::vector;
	/*----------------------------------声明--------------------------------------*/
	void loadNcnn(ncnn::Net& net_out, string param_path, string bin_path);
	
	vector<ncnn::Mat> quickRunNcnn(ncnn::Net& net_in, ncnn::Mat& input, string in_name, vector<string> out_names);

	vector<ncnn::Mat> quickRunNcnn(string param_path, string model_path, ncnn::Mat& input, string in_name, vector<string> out_names);

	template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
	void printMat(const ncnn::Mat& m, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });



	/*----------------------------------定义--------------------------------------*/
	inline void loadNcnn(ncnn::Net& net_out, string param_path, string bin_path)
	{
		using cs = ColorStr;
		//ncnn::Net net;
		if (net_out.load_param(param_path.c_str()))
			cout << cs::yellow(__func__, ":") << fstr(" 加载param文件{}失败！", param_path);
		if (net_out.load_model(bin_path.c_str()))
			cout << cs::yellow(__func__, ":") << fstr(" 加载bin文件{}失败！", bin_path);
	}
	
	inline vector<ncnn::Mat> quickRunNcnn(ncnn::Net& net_in, ncnn::Mat& input, string in_name, vector<string> out_names)
	{
		
		ncnn::Extractor extractor = net_in.create_extractor();
		extractor.input(in_name.c_str(), input);

		vector<ncnn::Mat> outs;
		ncnn::Mat out;
		for (string out_id : out_names)
		{
			extractor.extract(out_id.c_str(), out);
			outs.push_back(out);
		}
		return outs;
	}

	inline vector<ncnn::Mat> quickRunNcnn(string param_path, string model_path, ncnn::Mat& input, string in_name, vector<string> out_names)
	{
		ncnn::Net net;
		net.load_param(param_path.c_str());
		net.load_model(model_path.c_str());
		return quickRunNcnn(net, input, in_name, out_names);
	}


	template<class Tx, class Ty, class Tc>
	inline void printMat(const ncnn::Mat& m, Tx x_range, Ty y_range, Tc c_range)   //输出ncnn的Mat，只支持三维图像Mat
	{
		using Range = pair<int, int>;
		Range x_norm_range = normRange(x_range, m.w);
		Range y_norm_range = normRange(y_range, m.h);
		Range c_norm_range = normRange(c_range, m.c);
		int xstart = x_norm_range.first, xend = x_norm_range.second;
		int ystart = y_norm_range.first, yend = y_norm_range.second;
		int cstart = c_norm_range.first, cend = c_norm_range.second;

		const float* ptr = nullptr;
		int bias = ystart * m.w;
		cout << "ncnn::Mat{";
		for (int y = ystart; y < yend; y++)
		{
			if (y == ystart)
				cout << "[";
			else cout << string(10, ' ') << "[";
			for (int x = xstart; x < xend; x++)
			{
				cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					ptr = m.channel(c);
					if (c != cend - 1)
						cout <<  ptr[x + bias] << " ";
					else cout <<  ptr[x + bias];
				}
				if (x != xend - 1)
					cout << ") ";
				else cout << ")";
			}
			bias += m.w;
			if (y != yend - 1)
				cout << "]" << endl;
			else cout << "]";
		}
		cout << "}" << endl;
	}

	//std::ostream& operator<<(std::ostream& cout, ncnn::Mat& mat)
	//{
	//	printMat(mat);
	//	return cout;
	//}

	inline void printMats(const vector<ncnn::Mat>& mats)
	{
		cout << "Mats{" << endl;
		for (auto mat : mats)
			printMat(mat);
		cout << "}" << endl;
	}

	inline void _print(const ncnn::Mat& m)
	{
		printMat(m);
	}

	inline void _print(const vector<ncnn::Mat>& mats)
	{
		printMats(mats);
	}
}
