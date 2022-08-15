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
	using std::cout;
	using std::endl;
	using std::list;
	using std::map;
	using std::pair;
	using std::set;
	using std::string;
	using std::tuple;
	using std::vector;

	/*-------------------------------------����--------------------------------------*/
	template<class T, class... Args>
	void print(const T& arg, const Args&... args);

	void print();

	template<class T1, class T2>
	void _print(const pair<T1, T2>& pa);

	template<size_t Idx = 0, class... Ts>
	void _print(const tuple<Ts...>& tp);

	template<size_t Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1 tag);

	template<size_t Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0 tag);

	template<class T1, class T2, class... Ts>
	void _print(const map<T1, T2, Ts...>& m);

	template<class T, class... Ts>
	void _print(const list<T, Ts...>& l);

	template<class T, class... Ts>
	void _print(const set<T, Ts...>& st);

	template<class T, class... Ts>
	void _print(const vector<T, Ts...>& vec);

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	void _print_stdcter(const CTer<T, Ts...>& cter);

	void _print(const string& str);

	void _print(const bool& arg);

	template<class T>
	void _print(const T& arg);

	template<class T, int N>
	void _print(const T(&arr)[N]);

	/*-------------------------------------����--------------------------------------*/

	template<class T, class... Args>
	inline void print(const T& arg, const Args&... args)
	{	/*
		ʵ������python��print�����
		-֧��int��float��char��string�Ȼ����������ݵ������
		-֧��int[]��float[]�Ȼ�����ά�����������ݵ������
		-֧��vector��tuple�ȳ���STL�������ݵ������
		-֧���ⲿ��չ�Ե��������͵�֧�֣�����<<������_print����
		-Warning�����������֧���������ݵõ����������������쳣�Ŀ��ܡ�
		*/
		_print(arg);
		cout << " ";
		mineutils::print(args...);
		//int _a[] = { (_print(args), cout << " ", 0)... };   //��һ��д�������ɶ��Բ�һЩ
	}

	inline void print()
	{
		cout << endl;
	}

	template<class T1, class T2>
	inline void _print(const pair<T1, T2>& pa)  //���pair����
	{
		cout << "[";
		_print(pa.first);
		cout << " ";
		_print(pa.second);
		cout << "]";
	}

	template<size_t Idx, class... Ts>
	inline void _print(const tuple<Ts...>& tp)  //���tuple��
	{
		const int size_tp = std::tuple_size<tuple<Ts...>>::value;
		if (Idx == 0) 
			cout << "[";
		constexpr int type_id = (Idx < size_tp);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		_print_tuple<Idx, Ts...>(tp, size_tp, type_tag);
	}

	template<size_t Idx, class... Ts>
	inline void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1 tag)
	{
		if (Idx < size_tp - 1)
		{
			_print(std::get<Idx>(tp));
			cout << " ";
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
	inline void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0 tag)
	{
		cout << "]";
	}

	template<class T1, class T2, class... Ts>
	inline void _print(const map<T1, T2, Ts...>& m)  //���map��
	{
		cout << "{";
		int size = m.size();
		if (size > 0)
		{
			auto bg = m.begin();
			for (int i = 0; i < size - 1; ++i)
			{
				_print((*bg).first);
				cout << ": ";
				_print((*bg).second);
				cout << ", ";
				++bg;
			}
			_print((*bg).first);
			cout << ": ";
			_print((*bg).second);
		}
		cout << "}";
	}

	template<class T, class... Ts>
	inline void _print(const list<T, Ts...>& l)
	{
		_print_stdcter(l);
	}

	template<class T, class... Ts>
	inline void _print(const set<T, Ts...>& st)
	{
		_print_stdcter(st);
	}

	template<class T, class... Ts>
	inline void _print(const vector<T, Ts...>& vec)
	{
		_print_stdcter(vec);
	}

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	inline void _print_stdcter(const CTer<T, Ts...>& cter)
	{
		cout << "[";
		int size = cter.size();
		if (size > 0)
		{
			auto bg = cter.begin();
			for (int i = 0; i < size - 1; ++i)
			{
				_print(*bg);
				cout << " ";
				++bg;
			}
			_print(*bg);
		}
		cout << "]";
	}

	inline void _print(const string& str)
	{
		cout << str;
	}

	inline void _print(const bool& arg)
	{
		if (arg)
			cout << "true";
		else cout << "false";
	}

	template<class T>
	inline void _print(const T& arg)  //���int��char���Զ��������������
	{
		if (isInTypes<T, char, wchar_t>())
			cout << arg;
		else cout << arg;
	}

	template<class T, int N>
	inline void _print(const T(&arr)[N])  //���int[]��char[]����������
	{
		if (isInTypes<T, char, wchar_t>())
			cout << arr;
		else
		{
			cout << "[";
			for (int i = 0; i < N - 1; i++)
			{
				_print(arr[i]);
				cout << " ";
			}
			_print(arr[N - 1]);
			cout << "]";
		}
	}
}


