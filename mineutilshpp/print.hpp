#pragma once
#include<iostream>
#include<list>
#include<map>
#include<set>
#include<string>
#include<vector>

#include<assert.h>

#include"judge.hpp"


using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::tuple;
using std::vector;


/*-------------------------------------声明--------------------------------------*/
template<class T1, class T2>
void _print(const pair<T1, T2>& pa);

template<int Idx, class... Ts>
void _print(const tuple<Ts...>& tp);

template<class T1, class T2, class... Ts>
void _print(const map<T1, T2, Ts...>& m);

template<template<class C, class... Cs> class CTer, class T, class... Ts>
void _print(const CTer<T, Ts...>& nm);

void _print(const string& str);

template<class T>
void _print(const T& arg);

template<class T, int N>
void _print(const T(&arr)[N]);

template<class T, class... Args>
void print(const T& arg, const Args&... args);


/*-------------------------------------定义--------------------------------------*/
template<class T1, class T2>
void _print(const pair<T1, T2>& pa)  //输出pair类型
{
	cout << "Pair[";
	_print(pa.first);
	cout << " ";
	_print(pa.second);
	cout << "]";
}

template<int Idx = 0, class... Ts>
void _print(const tuple<Ts...>& tp)  //输出tuple类
{
	assert(Idx >= 0);
	const int size_tp = std::tuple_size<tuple<Ts...>>::value;
	if (Idx == 0)
		cout << "Tup[";

	if constexpr (Idx < size_tp - 1)
	{
		_print(std::get<Idx>(tp));
		cout << " ";
		_print<Idx + 1, Ts...>(tp);
	}
	else if constexpr (Idx == size_tp - 1)
	{
		_print(std::get<Idx>(tp));
		_print<Idx + 1, Ts...>(tp);
	}
	else
		cout << "]";
}

template<class T1, class T2, class... Ts>
void _print(const map<T1, T2, Ts...>& m)  //输出list类
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
	/*
	string class_name =  typeid(cter).name();
	size_t name_start_idx = class_name.find("std::", 0) + 5;
	size_t name_len = class_name.find("<", 0) - name_start_idx;
	string name = class_name.substr(name_start_idx, name_len);
	cout << name << "[";
	*/
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

void _print(const string& str)
{
	cout << str;
}

template<class T>
void _print(const T& arg)  //输出int、string等单数据类型
{
	if constexpr (isSameType<T, bool>())
	{
		if (arg)
			cout << "true";
		else
			cout << "false";
	}
	else 
		cout << arg;
}

template<class T, int N>
void _print(const T(&arr)[N])  //输出int[]、char[]等数组类型
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

template<class T, class... Args>
void print(const T& arg, const Args&... args)
{
	/*
	实现类似python的print输出。
	-支持int、float、char、string等基本类型数据的输出；
	-支持int[]、float[]等基本多维数组类型数据的输出；
	-支持vector、tuple、pair、list、set、map及其相互嵌套类型数据的输出；
	-Warning：存在输入非支持类型数据得到错误结果但不产生异常的可能。
	*/

	//int _a[] = { (_print(args), cout << " ", 0)... };   //另一种写法，但可读性差一些
	_print(arg);
	cout << " ";
	if constexpr (sizeof...(args) > 0)
		print(args...);
	else
		cout << endl;
}











