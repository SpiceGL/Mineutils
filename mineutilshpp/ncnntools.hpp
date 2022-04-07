#pragma once
#include<iostream>

#include<limits.h>
#include<net.h>
#include<stdlib.h>

#include"colorstr.hpp"
#include"index.hpp"
#include"judge.hpp"
#include"print.hpp"
#include"sign.hpp"


using std::cout;
using std::endl;
using std::pair;


namespace mineutils
{
	/*----------------------------------声明--------------------------------------*/
	template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
	void printMat(ncnn::Mat& m, Tx x_range = { 0, INT_MAX }, Ty y_range = { 0, INT_MAX }, Tc c_range = { 0, INT_MAX });



	/*----------------------------------定义--------------------------------------*/
	template<class Tx, class Ty, class Tc>
	void printMat(ncnn::Mat& m, Tx x_range, Ty y_range, Tc c_range)   //输出ncnn的Mat，只支持三维图像Mat
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
		if (yend - ystart == 1)
			cout << "ncnn::Mat{";
		else
			cout << "ncnn::Mat{" << endl;
		for (int y = ystart; y < yend; y++)
		{
			cout << "[";
			for (int x = xstart; x < xend; x++)
			{
				cout << "(";
				for (int c = cstart; c < cend; c++)
				{
					ptr = m.channel(c);
					if (c != cend - 1)
						cout << std::setw(3) << std::setprecision(5) << ptr[x + bias] << " ";
					else
						cout << std::setw(3) << std::setprecision(5) << ptr[x + bias];
				}
				if (x != xend - 1)
					cout << ") ";
				else
					cout << ")";
			}
			bias += m.w;
			if (y != yend - 1)
				cout << "]" << endl;
			else
				cout << "]";
		}
		cout << "}" << endl;
	}
}
