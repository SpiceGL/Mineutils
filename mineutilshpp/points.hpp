#pragma once
#include<cmath>
#include<initializer_list>
#include<iostream>
#include<string>
#include<stdlib.h>

#include"index.hpp"
#include"judge.hpp"
#include"str.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;



	template<class T>
	class BaseBox
	{
	public:
		using cs = ColorStr;
		using Box = std::initializer_list<T>;
		BaseBox(std::initializer_list<T> input_list)
		{
			if (input_list.size() != 4)
			{
				cout << cs::red("BaseBox::", __func__, ":") << " 输入长度必须为4！" << endl;
				exit(0);
			}
			int idx = 0;
			for (int value: input_list)
			{
				this->data[idx] = value;
				++idx;
			}
		}

	public:
		T data[4];

		T& operator[](int idx)
		{
			idx = normIdx(idx, 4);
			return this->data[idx];
		}

		T roundIntOnly(T x)
		{
			if (isInTypes<T, char, unsigned char, short, unsigned short, 
				int, unsigned int, long, unsigned long, long long, unsigned long long>())
				return floor(x + 0.5);
			else return x;
		}
	};


	/*左-上-右-下排列的bbox坐标*/
	template<class T>
	class LTRBBox :public BaseBox<T>
	{
	public:
		LTRBBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}

		T& left = this->data[0];
		T& top = this->data[1];
		T& right = this->data[2];
		T& bottom = this->data[3];

		XYWHBox<T> toXYWH()
		{
			T w = right - left;
			T h = bottom - top;
			T x = this->roundIntOnly((float)left + (float)w / 2);
			T y = this->roundIntOnly((float)top + (float)h / 2);
			return { x,y,w,h };
		}
	};


	template<class T>
	class XYWHBox :public BaseBox<T>
	{
	public:
		XYWHBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}

		T& x = this->data[0];
		T& y = this->data[1];
		T& width = this->data[2];
		T& height = this->data[3];

		LTRBBox<T> toLTRB()
		{
			T l = this->roundIntOnly(x - (float)width / 2);
			T t = this->roundIntOnly(y - (float)height / 2);
			T r = this->roundIntOnly(x + (float)width / 2);
			T b = this->roundIntOnly(y + (float)height / 2);
			return { l,t,r,b };
		}
	};


	/*------------------------------------------------------------------------------*/
	template<class T>
	std::ostream& operator<<(std::ostream& cout, const BaseBox<T>& bbox)   //添加对print的支持，bbox要么const &要么不引用
	{
		cout << "[" << bbox.data[0] << " " << bbox.data[1] << " "
			<< bbox.data[2] << " " << bbox.data[3] << "]";
		return cout;
	}
	//template<class T>
	//void _print(const BaseBox<T>& box)
	//{
	//	cout << "[" << bbox.data[0] << " " << bbox.data[1] << " "
	//		<< bbox.data[2] << " " << bbox.data[3] << "]";
	//}

	using LTRB = LTRBBox<int>; 
	using LTRB_F = LTRBBox<float>;
	using XYWH = XYWHBox<int>;
	using XYWH_F = XYWHBox<float>;
}
