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
	protected:
		T data[4];
	public:
		using cs = ColorStr;
		BaseBox(std::initializer_list<T> input_list)
		{
			if (input_list.size() != 4)
			{
				cout << cs::red("BaseBox::", __func__, ":") << " ���볤�ȱ���Ϊ4��" << endl;
				exit(0);
			}
			int idx = 0;
			for (int value : input_list)
			{
				this->data[idx] = value;
				++idx;
			}
		}

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

		/*�ڶ��������͵�ʱ����������*/
		static T roundWhenInt(float x)   
		{
			if (isInTypes<T, char, unsigned char, short, unsigned short,
				int, unsigned int, long, unsigned long, long long, unsigned long long>())
				return round(x);
			else return x;
		}
	};

	template<class T>
	class XYWHBox;

	template<class T>
	class LTRBBox;

	/*��-��-��-�����е�bbox����*/
	template<class T>
	class LTRBBox :public BaseBox<T>
	{
	public:
		LTRBBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}

		T& left = this->data[0];
		T& top = this->data[1];
		T& right = this->data[2];
		T& bottom = this->data[3];

		LTRBBox<int> toPixel() const
		{
			return { (int)round(left), (int)round(top), (int)round(right), (int)round(bottom) };
		}

		XYWHBox<T> toXYWH() const;
	};


	template<class T>
	class XYWHBox :public BaseBox<T>
	{
	public:
		XYWHBox(std::initializer_list<T> input_list) :BaseBox<T>(input_list) {}

		T& x = this->data[0];
		T& y = this->data[1];
		T& w = this->data[2];
		T& h = this->data[3];

		XYWHBox<int> toPixel() const
		{
			return { (int)round(x), (int)round(y), (int)round(w), (int)round(h) };
		}

		LTRBBox<T> toLTRB() const;
	};

	template<class T>
	XYWHBox<T> LTRBBox<T>::toXYWH() const
	{
		T w = this->right - this->left;
		T h = this->bottom - this->top;
		T x = BaseBox<T>::roundWhenInt(this->left + (float)w / 2);
		T y = BaseBox<T>::roundWhenInt(this->top + (float)h / 2);
		return { x,y,w,h };
	}

	template<class T>
	LTRBBox<T> XYWHBox<T>::toLTRB() const
	{
		T l = BaseBox<T>::roundWhenInt(this->x - (float)this->w / 2);
		T t = BaseBox<T>::roundWhenInt(this->y - (float)this->h / 2);
		T r = BaseBox<T>::roundWhenInt(this->x + (float)this->w / 2);
		T b = BaseBox<T>::roundWhenInt(this->y + (float)this->h / 2);
		return { l,t,r,b };
	}


	/*------------------------------------------------------------------------------*/
	template<class T>
	std::ostream& operator<<(std::ostream& cout, const BaseBox<T>& bbox)   //��Ӷ�print��֧�֣�bboxҪôconst &Ҫô������
	{
		cout << "[" << bbox[0] << " " << bbox[1] << " "
			<< bbox[2] << " " << bbox[3] << "]";
		return cout;
	}

	using LTRB = LTRBBox<int>;
	using LTRBf = LTRBBox<float>;
	using XYWH = XYWHBox<int>;
	using XYWHf = XYWHBox<float>;
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
//	using std::cout;
//	using std::endl;
//
//
//	template<class T>
//	class BaseBox
//	{
//	protected:
//		T data[4];
//	public:
//		using cs = ColorStr;
//		BaseBox(std::initializer_list<T> input_list)
//		{
//			if (input_list.size() != 4)
//			{
//				cout << cs::red("BaseBox::", __func__, ":") << " ���볤�ȱ���Ϊ4��" << endl;
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
//	/*��-��-��-�����е�bbox����*/
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
//	std::ostream& operator<<(std::ostream& cout, const BaseBox<T>& bbox)   //��Ӷ�print��֧�֣�bboxҪôconst &Ҫô������
//	{
//		cout << "[" << bbox[0] << " " << bbox[1] << " "
//			<< bbox[2] << " " << bbox[3] << "]";
//		return cout;
//	}
//
//	using LTRB = LTRBBox<int>; 
//	using LTRBf = LTRBBox<float>;
//	using XYWH = XYWHBox<int>;
//	using XYWHf = XYWHBox<float>;
//}
