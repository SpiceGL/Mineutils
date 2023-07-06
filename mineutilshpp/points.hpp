﻿#pragma once
#include<cmath>
#include<iostream>
#include<string>
#include<stdlib.h>

#include"str.hpp"
#include"exception.hpp"
#include"judge.hpp"
#include"index.hpp"


namespace mineutils
{
	template<class T>
	class BaseBox
	{
	protected:
		T data[4];
	public:
		BaseBox() 
		{ 
			static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "class T is not integral or floating_point!"); 
		}

		BaseBox(const T& v0, const T& v1, const T& v2, const T& v3)
		{
			static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "class T is not integral or floating_point!");
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}
		BaseBox(const BaseBox<T>& box)
		{
			this->data[0] = box.data[0];
			this->data[1] = box.data[1];
			this->data[2] = box.data[2];
			this->data[3] = box.data[3];
		}
		virtual ~BaseBox() {}

		T& operator[](int idx)
		{
			idx = normIdx(idx, 4);
			return this->data[idx];
		}

		T operator[](int idx) const
		{
			idx = normIdx(idx, 4);
			return this->data[idx];
		}

		static bool belongToIntSeries()
		{
			/*if (isInTypes<T, char, unsigned char, short, unsigned short,
				int, unsigned int, long, unsigned long, long long, unsigned long long>())*/
			if (std::is_integral<T>::value)
				return true;
			else return false;
		}
	};

	template<class T>
	class XYWHBox;

	template<class T>
	class LTRBBox;

	template<class T>
	class LTWHBox;

	/*左上角-右下角：l、t、r、b排列的bbox坐标*/
	template<class T>
	class LTRBBox :public BaseBox<T>
	{
	public:
		LTRBBox() :BaseBox<T>() {}
		LTRBBox(const T& v0, const T& v1, const T& v2, const T& v3) :BaseBox<T>(v0, v1, v2, v3) {}
		LTRBBox(const LTRBBox<T>& ltrb) :BaseBox<T>(ltrb) {}

		T& left = this->data[0];
		T& top = this->data[1];
		T& right = this->data[2];
		T& bottom = this->data[3];

		T& l = left;
		T& t = top;
		T& r = right;
		T& b = bottom;

		LTRBBox<T>& operator=(const LTRBBox<T>& ltrb)
		{
			left = ltrb.left;
			top = ltrb.top;
			right = ltrb.right;
			bottom = ltrb.bottom;
			return *this;
		}

		//将坐标转化为像素坐标
		template<class PT = int>
		LTRBBox<PT> toPixel() const
		{
			return { PT(floor(left)), PT(floor(top)), PT(floor(right)), PT(floor(bottom)) };
		}

		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		XYWHBox<T> toXYWH() const;
		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		LTWHBox<T> toLTWH() const;
	};

	/*中心-宽高：x、y、w、h排列的bbox坐标*/
	template<class T>
	class XYWHBox :public BaseBox<T>
	{
	public:
		XYWHBox() :BaseBox<T>() {}
		XYWHBox(const T& v0, const T& v1, const T& v2, const T& v3) :BaseBox<T>(v0, v1, v2, v3) {}
		XYWHBox(const XYWHBox<T>& xywh) :BaseBox<T>(xywh) {}

		T& x = this->data[0];
		T& y = this->data[1];
		T& w = this->data[2];
		T& h = this->data[3];

		XYWHBox<T>& operator=(const XYWHBox<T>& xywh)
		{
			x = xywh.x;
			y = xywh.y;
			w = xywh.w;
			h = xywh.h;
			return *this;
		}

		//将坐标转化为像素坐标
		template<class PT = int>
		XYWHBox<PT> toPixel() const
		{
			return { PT(floor(x)), PT(floor(y)), PT(floor(w)), PT(floor(h)) };
		}

		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		LTRBBox<T> toLTRB() const;
		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		LTWHBox<T> toLTWH() const;
	};

	/*左上角-宽高：l、t、w、h排列的bbox坐标*/
	template<class T>
	class LTWHBox :public BaseBox<T>
	{
	public:
		LTWHBox() :BaseBox<T>() {}
		LTWHBox(const T& v0, const T& v1, const T& v2, const T& v3) :BaseBox<T>(v0, v1, v2, v3) {}
		LTWHBox(const LTWHBox<T>& ltwh) :BaseBox<T>(ltwh) {}

		T& left = this->data[0];
		T& top = this->data[1];
		T& w = this->data[2];
		T& h = this->data[3];

		T& l = left;
		T& t = top;

		LTWHBox<T>& operator=(const LTWHBox<T>& ltwh)
		{
			l = ltwh.l;
			t = ltwh.t;
			w = ltwh.w;
			h = ltwh.h;
			return *this;
		}

		/*将坐标转化为像素坐标，向下取整*/
		template<class PT = int>
		LTWHBox<PT> toPixel() const
		{
			return { PT(floor(l)), PT(floor(t)), PT(floor(w)), PT(floor(h)) };
		}

		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		LTRBBox<T> toLTRB() const;
		//若自身数据为整数类型，则按像素运算，否则按连续数值运算
		XYWHBox<T> toXYWH() const;
	};

	/*-----------------------------------------------*/
	template<class T>
	XYWHBox<T> LTRBBox<T>::toXYWH() const
	{

		if (BaseBox<T>::belongToIntSeries())
		{
			
			T _w = this->right - this->left + 1;
			T _h = this->bottom - this->top + 1;
			T _x = this->left + ceil(double(_w - 1) / 2);
			T _y = this->top + ceil(double(_h - 1) / 2);
			return { _x,_y,_w,_h };
		}
		else
		{
			T _w = this->right - this->left;
			T _h = this->bottom - this->top;
			T _x = this->left + _w / 2;
			T _y = this->top + _h / 2;
			return { _x,_y,_w,_h };
		}
	}

	template<class T>
	LTWHBox<T> LTRBBox<T>::toLTWH() const
	{
		T _w = this->right - this->left;
		T _h = this->bottom - this->top;
		if (BaseBox<T>::belongToIntSeries())
		{
			_w += 1;
			_h += 1;
		}
		return { this->l, this->t, _w, _h };
	}

	/*-----------------------------------------------*/
	template<class T>
	LTRBBox<T> XYWHBox<T>::toLTRB() const
	{
		if (BaseBox<T>::belongToIntSeries())
		{
			T _l = this->x - ceil(double(this->w - 1) / 2);
			T _t = this->y - ceil(double(this->h - 1) / 2);
			T _r = _l + this->w - 1;
			T _b = _t + this->h - 1;
			return { _l,_t,_r,_b };
		}
		else
		{
			T _l = this->x - this->w / 2;
			T _t = this->y - this->h / 2;
			T _r = _l + this->w;
			T _b = _t + this->h;
			return { _l,_t,_r,_b };
		}
	}

	template<class T>
	LTWHBox<T> XYWHBox<T>::toLTWH() const
	{
		if (BaseBox<T>::belongToIntSeries())
		{
			T _l = this->x - ceil(double(this->w - 1) / 2);
			T _t = this->y - ceil(double(this->h - 1) / 2);
			return { _l, _t, this->w, this->h };
		}
		else
		{
			T _l = this->x - this->w / 2;
			T _t = this->y - this->h / 2;
			return { _l, _t, this->w, this->h };
		}
	}

	/*-----------------------------------------------*/
	template<class T>
	LTRBBox<T> LTWHBox<T>::toLTRB() const
	{
		T r = this->left + this->w;
		T b = this->top + this->h;
		if (BaseBox<T>::belongToIntSeries())
		{
			r -= 1;
			b -= 1;
		}
		return { this->l, this->t, r, b };
	}

	template<class T>
	XYWHBox<T> LTWHBox<T>::toXYWH() const
	{
		if (BaseBox<T>::belongToIntSeries())
		{
			T _x = this->left + ceil(double(this->w - 1) / 2);
			T _y = this->top + ceil(double(this->h - 1) / 2);
			return { _x, _y, this->w, this->h };
		}
		else
		{
			T _x = this->left + this->w / 2;
			T _y = this->top + this->h / 2;
			return { _x, _y, this->w, this->h };
		}
		
	}

	/*------------------------------------------------------------------------------*/
	template<class T>
	inline std::ostream& operator<<(std::ostream& _cout, const BaseBox<T>& bbox)   //添加对print的支持，bbox要么const &要么不引用
	{
		std::cout << "[" << bbox[0] << " " << bbox[1] << " "
			<< bbox[2] << " " << bbox[3] << "]";
		return std::cout;
	}

	using LTRB = LTRBBox<int>;
	using LTRBf = LTRBBox<float>;
	using XYWH = XYWHBox<int>;
	using XYWHf = XYWHBox<float>;
	using LTWH = LTWHBox<int>;
	using LTWHf = LTWHBox<float>;
}

//#pragma once
//#include<cmath>
//#include<initializer_list>
//#include<iostream>
//#include<string>
//#include<stdlib.h>
//
//#include"index.hpp"
//#include"judge.hpp"
//#include"str.hpp"
//
//
//namespace mineutils
//{
//	using std::std::cout;
//	using std::std::endl;
//
//
//	template<class T>
//	class BaseBox
//	{
//	protected:
//		T data[4];
//	public:
//		using cstr = ColorStr;
//		BaseBox(std::initializer_list<T> input_list)
//		{
//			if (input_list.size() != 4)
//			{
//				std::cout << cstr::red("BaseBox::", __func__, ":") << " 输入长度必须为4！" << std::endl;
//				exit(0);
//			}
//			int idx = 0;
//			for (int value: input_list)
//			{
//				this->data[idx] = value;
//				++idx;
//			}
//		}
//
//		T& operator[](int idx)
//		{
//			idx = normIdx(idx, 4);
//			return this->data[idx];
//		}
//
//		const T& operator[](int idx) const
//		{
//			idx = normIdx(idx, 4);
//			return this->data[idx];
//		}
//
//		static T roundWhenInt(float x)
//		{
//			if (isInTypes<T, char, unsigned char, short, unsigned short, 
//				int, unsigned int, long, unsigned long, long long, unsigned long long>())
//				return round(x);
//			else return x;
//		}
//	};
//
//
//	/*左-上-右-下排列的bbox坐标*/
//	template<class T>
//	class LTRBBox :public BaseBox<T>
//	{
//	public:
//		LTRBBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}
//
//		T& left = this->data[0];
//		T& top = this->data[1];
//		T& right = this->data[2];
//		T& bottom = this->data[3];
//
//		LTRBBox<int> toPixel()
//		{
//			return { (int)round(left), (int)round(top), (int)round(right), (int)round(bottom) };
//		}
//
//
//	};
//
//
//	template<class T>
//	class XYWHBox :public BaseBox<T>
//	{
//	public:
//		XYWHBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}
//
//		T& x = this->data[0];
//		T& y = this->data[1];
//		T& width = this->data[2];
//		T& height = this->data[3];
//
//		XYWHBox<int> toPixel()
//		{
//			return { (int)round(x), (int)round(y), (int)round(width), (int)round(height) };
//		}
//
//	};
//
//
//	template<class T>
//	LTRBBox<T> toLTRB(const XYWHBox<T>& box)
//	{
//		T l = BaseBox<T>::roundWhenInt(box.x - (float)box.width / 2);
//		T t = BaseBox<T>::roundWhenInt(box.y - (float)box.height / 2);
//		T r = BaseBox<T>::roundWhenInt(box.x + (float)box.width / 2);
//		T b = BaseBox<T>::roundWhenInt(box.y + (float)box.height / 2);
//		return { l,t,r,b };
//	}
//
//	template<class T>
//	XYWHBox<T> toXYWH(const LTRBBox<T>& box)
//	{
//		T w = box.right - box.left;
//		T h = box.bottom - box.top;
//		T x = BaseBox<T>::roundWhenInt(box.left + (float)w / 2);
//		T y = BaseBox<T>::roundWhenInt(box.top + (float)h / 2);
//		return { x,y,w,h };
//	}
//
//	/*------------------------------------------------------------------------------*/
//	template<class T>
//	std::ostream& operator<<(std::ostream& std::cout, const BaseBox<T>& bbox)   //添加对print的支持，bbox要么const &要么不引用
//	{
//		std::cout << "[" << bbox[0] << " " << bbox[1] << " "
//			<< bbox[2] << " " << bbox[3] << "]";
//		return std::cout;
//	}
//
//	using LTRB = LTRBBox<int>; 
//	using LTRBf = LTRBBox<float>;
//	using XYWH = XYWHBox<int>;
//	using XYWHf = XYWHBox<float>;
//}
