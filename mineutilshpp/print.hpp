#pragma once
#include<iostream>
#include<list>
#include<map>
#include<set>
#include<string>
#include<vector>

#include<assert.h>

#include"judge.hpp"
#include"sign.hpp"


using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::tuple;
using std::vector;

namespace mineutils
{
	/*-------------------------------------����--------------------------------------*/
	template<class T, class... Args>
	void print(const T& arg, const Args&... args);

	void print();

	template<class T1, class T2>
	void _print(const pair<T1, T2>& pa);

	template<int Idx = 0, class... Ts>
	void _print(const tuple<Ts...>& tp);

	template<int Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1& tag);

	template<int Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0& tag);

	template<class T1, class T2, class... Ts>
	void _print(const map<T1, T2, Ts...>& m);

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	void _print(const CTer<T, Ts...>& nm);

	void _print(const string& str);

	void _print(const bool& arg);

	template<class T>
	void _print(const T& arg);

	template<class T, int N>
	void _print(const T(&arr)[N]);

	/*-------------------------------------����--------------------------------------*/

	template<class T, class... Args>
	void print(const T& arg, const Args&... args)
	{	/*
		ʵ������python��print�����
		-֧��int��float��char��string�Ȼ����������ݵ������
		-֧��int[]��float[]�Ȼ�����ά�����������ݵ������
		-֧��vector��tuple��STL�������ݵ������
		-֧���ⲿ��չ�Ե��������͵�֧�֣�����<<����������
		-Warning�����������֧���������ݵõ����������������쳣�Ŀ��ܡ�
		*/
		_print(arg);
		cout << " ";
		print(args...);
		//int _a[] = { (_print(args), cout << " ", 0)... };   //��һ��д�������ɶ��Բ�һЩ
	}

	void print()
	{
		cout << endl;
	}

	template<class T1, class T2>
	void _print(const pair<T1, T2>& pa)  //���pair����
	{
		cout << "Pair[";
		_print(pa.first);
		cout << " ";
		_print(pa.second);
		cout << "]";
	}

	template<int Idx, class... Ts>
	void _print(const tuple<Ts...>& tp)  //���tuple��
	{
		assert(Idx >= 0);
		const int size_tp = std::tuple_size<tuple<Ts...>>::value;
		if (Idx == 0) 
			cout << "Tup[";
		constexpr int type_id = (Idx < size_tp);
		auto& type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		_print_tuple<Idx, Ts...>(tp, size_tp, type_tag);
	}

	template<int Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag1& tag)
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

	template<int Idx, class... Ts>
	void _print_tuple(const tuple<Ts...>& tp, const int& size_tp, Sign::CaseTag0& tag)
	{
		cout << "]";
	}

	template<class T1, class T2, class... Ts>
	void _print(const map<T1, T2, Ts...>& m)  //���list��
	{
		cout << "map{";
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

	template<template<class C, class... Cs> class CTer, class T, class... Ts>
	void _print(const CTer<T, Ts...>& cter)
	{
		cout << "CTer[";
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

	void _print(const string& str)
	{
		cout << "\"" << str << "\"";
	}

	void _print(const bool& arg)
	{
		if (arg)
			cout << "true";
		else cout << "false";
	}

	template<class T>
	void _print(const T& arg)  //���int��string�ȵ���������
	{
		if (isInTypes<T, char, wchar_t>())
			cout << "\'" << arg << "\'";
		else cout << arg;
	}

	template<class T, int N>
	void _print(const T(&arr)[N])  //���int[]��char[]����������
	{
		if (isInTypes<T, char, wchar_t>())
			cout << "\"" << arr << "\"";
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


