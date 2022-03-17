#pragma once
#include<iostream>

#include<net.h>


using std::cout;
using std::endl;


void printNCNNMat(ncnn::Mat m, int xstart = 0, int xend = 5, int ystart = 0, int yend = 5)
{
	const float* ptr = nullptr;

	for (int y = ystart; y < yend; y++)
	{
		for (int x = xstart; x < xend; x++)
		{
			cout << "ncnn::Mat[";
			for (int q = 0; q < m.c; q++)
			{
				ptr = m.channel(q);
			}
		}
		ptr += m.w;
		cout << endl;
	}
}