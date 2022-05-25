#pragma once
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include"colorstr.hpp"
#include"judge.hpp"
#include"print.hpp"

using std::string;
using std::vector;

namespace mineutils
{
	/*---------------------------------声明-------------------------------------*/

	/*---------------------------------定义-------------------------------------*/

	template<class T>
	string toStr(const T& arg)
	{
		std::ostringstream buffer;
		buffer << arg;
		return buffer.str();
	}
	
	string int2str(int n, int len = 0)
	{
		string s = std::to_string(n);
		if (s.length() < len)
		{
			s = string(len - s.length(), '0') + s;
		}
		return s;
	}

	/*--------------------------------------------------------------------------*/

	string _fstr(string& s)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			print(cs::red(__func__, ":"), "fstring待替换参数量过少，程序已中止！");
			exit(0);
		}
		return s;
	}
	
	template<class T, class... Ts>
	string _fstr(string& s, T& arg, Ts&... args)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			string s_arg = toStr(arg);
			s.replace(pos, 2, s_arg);
		}
		else
		{
			print(cs::red(__func__, ":"), "fstring待替换参数量过多，程序已中止！");
			exit(0);
		}
		return _fstr(s, args...);
	}

	template<class... Ts>
	string fstr(string s, Ts... args)
	{
		return _fstr(s, args...);	
	}

	vector<string> split(string s, string sep=" ", bool ignore_emptystr=true)
	{
		vector<string> strs;
		size_t sep_pos;
		string s0 = s, s1;   //s0存放已处理的字段，s1存放未处理的字段
		while ((sep_pos = s0.find(sep)) != -1)
		{
			s0 = s.substr(0, sep_pos);
			s1 = s.substr(sep_pos + sep.length());
			if ((ignore_emptystr and s0.length() != 0) or !ignore_emptystr)
				strs.push_back(s0);
			s = s1, s0 = s;
		}
		if ((ignore_emptystr and s0.length() != 0) or !ignore_emptystr)
			strs.push_back(s0);
		return strs;
	}


	/*-----------------------------------废弃代码----------------------------------*/
	/*
	string toStr(const char* s)
	{
		return s;
	}

	string toStr(const char& c)
	{
		string s;
		s.push_back(c);
		return s;
	}

	string toStr(const string& s)
	{
		return s;
	}

	template<class T>
	string toStr(const T& arg)
	{
		string s = std::to_string(arg);
		//去掉小数末尾的0
		size_t point_pos = s.find(".");
		if (point_pos != -1)
		{
			size_t end_pos = 0;
			for (int i = 1; i <= 9; ++i)
			{
				size_t i_pos = s.rfind(std::to_string(i));
				if (i_pos != -1 and i_pos > end_pos)
					end_pos = i_pos;
			}
			if (point_pos < end_pos)
				s = s.substr(0, end_pos + 1);
		}
		return s;
	}
	
	
	*/

}

