#pragma once
#include<exception>
#include<iostream>
#include<list>
#include<map>
#include<set>
#include<string>
#include<vector>

#include<assert.h>

#include"judge.hpp"
#include"sign.hpp"


namespace mineutils
{
	/*-------------------------------------����--------------------------------------*/
	template<class T, class... Args>
	void print(const T& arg, const Args&... args);

	void print();

	template<class T1, class T2>
	void _print(const std::pair<T1, T2>& pa);

	template<size_t Idx = 0, class... Ts>
	void _print(const std::tuple<Ts...>& tp);

	template<size_t Idx, class... Ts>
	void _print_tuple(const std::tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1 tag);

	template<size_t Idx, class... Ts>
	void _print_tuple(const std::tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0 tag);

	template<class T1, class T2, class... Ts>
	void _print(const std::map<T1, T2, Ts...>& m);

	template<class T, class... Ts>
	void _print(const std::list<T, Ts...>& l);

	template<class T, class... Ts>
	void _print(const std::set<T, Ts...>& st);

	template<class T, class... Ts>
	void _print(const std::vector<T, Ts...>& vec);

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	void _print_stdcter(const CTer<T, Ts...>& cter);

	void _print(const std::string& str);

	void _print(const bool& arg);

	template<class T>
	void _print(const T& arg);

	template<class T, int N>
	void _print(const T(&arr)[N]);

	/*-------------------------------------����--------------------------------------*/

	/*
		ʵ������python��print�����
		-֧��int��float��char��std::string�Ȼ����������ݵ������
		-֧��int[]��float[]�Ȼ�����ά�����������ݵ������
		-֧��std::vector��std::tuple�ȳ���STL�������ݵ������
		-֧���ⲿ��չ�Ե��������͵�֧�֣�����<<������_print����
		-Warning�����������֧���������ݵõ����������������쳣�Ŀ��ܡ�
	*/
	template<class T, class... Args>
	inline void print(const T& arg, const Args&... args)
	{	
		_print(arg);
		std::cout << " ";
		mineutils::print(args...);
		//int _a[] = { (_print(args), std::cout << " ", 0)... };   //��һ��д�������ɶ��Բ�һЩ
	}

	inline void print()
	{
		std::cout << std::endl;
	}

	template<class T1, class T2>
	inline void _print(const std::pair<T1, T2>& pa)  //���std::pair����
	{
		std::cout << "[";
		_print(pa.first);
		std::cout << " ";
		_print(pa.second);
		std::cout << "]";
	}

	template<size_t Idx, class... Ts>
	inline void _print(const std::tuple<Ts...>& tp)  //���std::tuple��
	{
		const int size_tp = std::tuple_size<std::tuple<Ts...>>::value;
		if (Idx == 0) 
			std::cout << "[";

		constexpr int type_id = (Idx < size_tp);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		_print_tuple<Idx, Ts...>(tp, size_tp, type_tag);
	}

	template<size_t Idx, class... Ts>
	inline void _print_tuple(const std::tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1 tag)
	{
		if (Idx < size_tp - 1)
		{
			_print(std::get<Idx>(tp));
			std::cout << " ";
			_print<Idx + 1, Ts...>(tp);
		}
		else if (Idx == size_tp - 1)
		{
			_print(std::get<Idx>(tp));
			_print<Idx + 1, Ts...>(tp);
		}
		else throw "Error Idx!";
	}

	template<size_t Idx, class... Ts>
	inline void _print_tuple(const std::tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0 tag)
	{
		std::cout << "]";
	}

	template<class T1, class T2, class... Ts>
	inline void _print(const std::map<T1, T2, Ts...>& m)  //���std::map��
	{
		std::cout << "{";
		int size = m.size();
		if (size > 0)
		{
			auto bg = m.begin();
			for (int i = 0; i < size - 1; ++i)
			{
				_print((*bg).first);
				std::cout << ": ";
				_print((*bg).second);
				std::cout << ", ";
				++bg;
			}
			_print((*bg).first);
			std::cout << ": ";
			_print((*bg).second);
		}
		std::cout << "}";
	}

	template<class T, class... Ts>
	inline void _print(const std::list<T, Ts...>& l)
	{
		_print_stdcter(l);
	}

	template<class T, class... Ts>
	inline void _print(const std::set<T, Ts...>& st)
	{
		_print_stdcter(st);
	}

	template<class T, class... Ts>
	inline void _print(const std::vector<T, Ts...>& vec)
	{
		_print_stdcter(vec);
	}

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	inline void _print_stdcter(const CTer<T, Ts...>& cter)
	{
		std::cout << "[";
		int size = cter.size();
		if (size > 0)
		{
			auto bg = cter.begin();
			for (int i = 0; i < size - 1; ++i)
			{
				_print(*bg);
				std::cout << " ";
				++bg;
			}
			_print(*bg);
		}
		std::cout << "]";
	}

	inline void _print(const std::string& str)
	{
		std::cout << str;
	}

	inline void _print(const bool& arg)
	{
		if (arg)
			std::cout << "true";
		else std::cout << "false";
	}

	template<class T>
	inline void _print(const T& arg)  //���int��char���Զ��������������
	{
		if (isInTypes<T, char, wchar_t>())
			std::cout << arg;
		else std::cout << arg;
	}

	template<class T, int N>
	inline void _print(const T(&arr)[N])  //���int[]��char[]����������
	{
		if (isInTypes<T, char, wchar_t>())
			std::cout << arr;
		else
		{
			std::cout << "[";
			for (int i = 0; i < N - 1; i++)
			{
				_print(arr[i]);
				std::cout << " ";
			}
			_print(arr[N - 1]);
			std::cout << "]";
		}
	}
}


