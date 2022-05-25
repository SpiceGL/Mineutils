#pragma once
#include<iostream>
#include<string>
#include<limits.h>
#include<stdlib.h>

#include"colorstr.hpp"
#include"index.hpp"
#include"judge.hpp"
#include"net.h"
#include"print.hpp"
#include"sign.hpp"


using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;


namespace mineutils
{
	/*----------------------------------����--------------------------------------*/
	vector<ncnn::Mat> quickRunNcnn(string param_path, string model_path, ncnn::Mat& in, int in_id, vector<int> out_ids);

	template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
	void printMat(const ncnn::Mat& m, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });



	/*----------------------------------����--------------------------------------*/

	vector<ncnn::Mat> quickRunNcnn(string param_path, string model_path, ncnn::Mat& in, int in_id, vector<int> out_ids)
	{
		ncnn::Net net;
		net.load_param(param_path.c_str());
		net.load_model(model_path.c_str());
		ncnn::Extractor extractor = net.create_extractor();
		extractor.input(in_id, in);

		vector<ncnn::Mat> outs;
		ncnn::Mat out;
		for (int out_id : out_ids)
		{
			extractor.extract(out_id, out);
			outs.push_back(out);
		}
		return outs;
	}


	template<class Tx, class Ty, class Tc>
	void printMat(const ncnn::Mat& m, Tx x_range, Ty y_range, Tc c_range)   //���ncnn��Mat��ֻ֧����άͼ��Mat
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
			else cout << std::setw(11) << "[";
			for (int x = xstart; x < xend; x++)
			{
				cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					ptr = m.channel(c);
					if (c != cend - 1)
						cout << std::setw(3) << std::setprecision(5) << ptr[x + bias] << " ";
					else cout << std::setw(3) << std::setprecision(5) << ptr[x + bias];
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

	void printMats(const vector<ncnn::Mat>& mats)
	{
		cout << "Mats{" << endl;
		for (auto mat : mats)
			printMat(mat);
		cout << "}" << endl;
	}
}
