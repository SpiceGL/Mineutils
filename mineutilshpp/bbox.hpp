#pragma once
#include<initializer_list>
#include<iostream>
#include<string>
#include<stdlib.h>

#include"colorstr.hpp"
#include"index.hpp"
#include"print.hpp"


namespace mineutils
{
	class BaseBbox
	{
	public:
		using cs = ColorStr;
		using Bbox = std::initializer_list<int>;

		BaseBbox(const Bbox& input_list)
		{
			if (input_list.size() != 4)
			{
				print(cs::red("BaseBbox::", __func__, ":"), "输入长度必须为4！");
				exit(0);
			}
			int idx = 0;
			for (int value : input_list)
			{
				this->data[idx] = value;
				++idx;
			}
		}

	public:
		int data[4];

		int operator[](int idx)
		{
			idx = normIdx(idx, 4);
			return this->data[idx];
		}
	};


	class LTRB :public BaseBbox
	{
	public:
		LTRB(const Bbox& input_list) :BaseBbox(input_list) {}

		int& left = data[0];
		int& top = data[1];
		int& right = data[2];
		int& bottom = data[3];
	};


	class XYWH :public BaseBbox
	{
	public:
		XYWH(const Bbox& input_list) :BaseBbox(input_list) {}

		int& x = data[0];
		int& y = data[1];
		int& w = data[2];
		int& h = data[3];
	};


	/*------------------------------------------------------------------------------*/

	std::ostream& operator<<(std::ostream& cout, const BaseBbox& bbox)   //添加对print的支持
	{
		_print(bbox.data);
		return cout;
	}
}
