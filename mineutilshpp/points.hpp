#pragma once
#include<initializer_list>
#include<iostream>
#include<string>
#include<stdlib.h>

#include"str.hpp"
#include"index.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;


	class BaseBox
	{
	public:
		using cs = ColorStr;
		using Box = std::initializer_list<int>;
		BaseBox(const Box& input_list)
		{
			if (input_list.size() != 4)
			{
				cout << cs::red("BaseBox::", __func__, ":") << " 输入长度必须为4！" << endl;
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

	/*左-上-右-下排列的bbox坐标*/
	class LTRB :public BaseBox
	{
	public:
		LTRB(const Box& input_list) :BaseBox(input_list) {}

		int& left = data[0];
		int& top = data[1];
		int& right = data[2];
		int& bottom = data[3];
	};


	class XYWH :public BaseBox
	{
	public:
		XYWH(const Box& input_list) :BaseBox(input_list) {}

		int& x = data[0];
		int& y = data[1];
		int& w = data[2];
		int& h = data[3];
	};


	/*------------------------------------------------------------------------------*/
	//std::ostream& operator<<(std::ostream& cout, const BaseBbox& bbox)   //添加对print的支持，bbox要么const &要么不引用
	//{
	//	_print(bbox.data);
	//	return cout;
	//}
	void _print(const BaseBox& bbox)
	{
		cout << bbox.data[0] << " " << bbox.data[1] << " "
			<< bbox.data[2] << " " << bbox.data[3];
	}

}
