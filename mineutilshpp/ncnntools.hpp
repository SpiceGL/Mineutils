#pragma once
#include<iostream>

#include<net.h>

#include"judge.hpp"


using std::cout;
using std::endl;
using std::pair;


template<class Tx = pair<int, int>, class Ty = pair<int, int>, class Tc = pair<int, int>>
void printNCNNMat(ncnn::Mat& m, Tx x_range, Ty y_range, Tc c_range)
{
	//if (not (isSameType<Tx, pair<int, int>> and isSameType<Ty, pair<int, int>> and isSameType<Tc, pair<int, int>>))

	const float* ptr = nullptr;

	cout << "ncnn::Mat[";
	for (int y = ystart; y < yend; y++)
	{
		for (int x = xstart; x < xend; x++)
		{
			
			for (int q = 0; q < m.c; q++)
			{
				ptr = m.channel(q);
				cout << ptr[x] << " ";
			}
		}
		ptr += m.w;
		cout << endl;
	}
	cout << "]" << endl;
}