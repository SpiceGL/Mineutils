#pragma once
#include<algorithm>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<stdlib.h>

namespace mineutils
{
	/*---------------------------------声明-------------------------------------*/

	/*---------------------------------定义-------------------------------------*/
	static bool ColorStr_enabled = false;   //在main函数cpp中更改，就可以影响全局，否则不影响
	

	class ColorStr   //似乎只在Windows和Linux桌面端生效
	{
	private:
		ColorStr() {}
	public:
		template<class ...Strs>
		static std::string black(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[30m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string blue(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[34m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string cyan(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[36m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string fuchsia(const std::string& str, const Strs& ...strs)  //
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[35m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string green(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[32m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string red(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[31m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string white(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[37m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static std::string yellow(const std::string& str, const Strs& ...strs)
		{
			std::string color_str;
			if (ColorStr_enabled)
				color_str = "\033[33m" + ColorStr::cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::cat(str, strs...);
			return color_str;
		}

	private:
		template<class ...Strs>
		static std::string cat(const std::string& str, const Strs& ...strs)
		{
			return str + ColorStr::cat(strs...);
		}

		static std::string cat(const std::string& str)
		{
			return str;
		}
	};

	using cstr = ColorStr;

	

	template<class T>
	inline std::string toStr(const T& arg)
	{
		static std::ostringstream str_buf;   //给toStr函数使用的字符串流
		str_buf.clear();
		str_buf.str("");
		str_buf << arg;
		return str_buf.str();
	}
	
	inline std::string zfillInt(int n, int min_len = 0, char padding = '0')   //输入int值，返回len长度的字符串
	{
		std::string s = std::to_string(n);
		if (s.length() < min_len)
		{
			s = std::string(min_len - s.length(), padding) + s;
		}
		return s;
	}

	inline std::string zfillFlt(float f, int min_len_int = 0, int flt_precision = 4,
		char int_padding = ' ', char flt_padding = '0')
	{
		std::ostringstream buffer;
		buffer << std::setprecision(flt_precision) << f;
		std::string s = buffer.str();

		/*找到输入小数的整数部分和小数部分，分别处理并合并*/
		std::string int_part, flt_part;
		size_t point_pos = s.find(".");
		if (point_pos != -1)
		{
			int_part = s.substr(0, point_pos);
			flt_part = s.substr(point_pos + 1);
		}
		else
		{
			int_part = s;
			flt_part = "";
		}
		if (int_part.length() < min_len_int)
			int_part = std::string(min_len_int - int_part.length(), int_padding) + int_part;
		if (flt_part.length() < flt_precision)
			flt_part = flt_part + std::string(flt_precision - flt_part.length(), flt_padding);
		s = int_part + "." + flt_part;
		return s;
	}

	/*--------------------------------------------------------------------------*/

	inline std::string _fstr(std::string& s)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			std::cout << cs::red("!!!Error!!! ", __func__, ": ") << "fstr待替换参数量过少！" << std::endl;
			return "";
		}
		return s;
	}
	
	template<class T, class... Ts>
	inline std::string _fstr(std::string& s, const T& arg, const Ts&... args)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			std::string arg_s = toStr(arg);
			s.replace(pos, 2, arg_s);
		}
		else
		{
			std::cout << cs::red("!!!Error!!! ", __func__, ": ") << "fstr待替换参数量过多！" << std::endl;
			return "";
		}
		return _fstr(s, args...);
	}

	template<class... Ts>
	inline std::string fstr(std::string s, const Ts& ...args)   //实现类似于python的f-std::string功能，将字符串中的{}替换为后面的参数
	{
		return _fstr(s, args...);	
	}
	
	////实现对字符串的分割功能，默认会保留空字符串，以vector形式返回，至少会返回包含原字符串的vector
	//inline std::vector<std::string> split(std::string s, const std::string& sep=" ", bool ignore_emptystr=false)
	//{
	//	std::vector<std::string> strs;
	//	size_t sep_pos;
	//	std::string s_in;   //s_in存放已处理的字段，s存放待处理的字段

	//	if ((sep_pos = s.find(sep)) != -1)
	//	{
	//		while ((sep_pos = s.find(sep)) != -1)
	//		{
	//			s_in = s.substr(0, sep_pos);
	//			s = s.substr(sep_pos + sep.length());
	//			if ((ignore_emptystr and s_in.length() != 0) or !ignore_emptystr)
	//				strs.push_back(s_in);
	// 			s_in.clear();
	//		}
	//		if ((ignore_emptystr and s.length() != 0) or !ignore_emptystr)
	//			strs.push_back(s);
	//	}
	//	else strs.push_back(s);
	//	return strs;
	//}

	//实现对字符串的分割功能，以vector形式返回
	inline std::vector<std::string> split(std::string s, const std::string& sep)
	{
		std::vector<std::string> strs;
		size_t sep_pos;
		std::string s_in;   //s_in存放已处理的字段，s存放待处理的字段

		while ((sep_pos = s.find(sep)) != -1)
		{
			s_in = s.substr(0, sep_pos);
			s = s.substr(sep_pos + sep.length());
			strs.push_back(s_in);
			s_in.clear();
		}
		strs.push_back(s);
		return strs;
	}

	//按字符串中的空格分割字符串，至少返回包含一个空字符串的vector
	inline std::vector<std::string> split(std::string s)
	{
		std::vector<std::string> strs;
		std::stringstream ss;
		ss << s;
		std::string s_in;
		ss >> s_in;
		do
		{
			strs.push_back(s_in);
			s_in.clear();
		} while (ss >> s_in);
		return strs;
	}


	/*-----------------------------------废弃代码----------------------------------*/
	/*
	std::string toStr(const char* s)
	{
		return s;
	}

	std::string toStr(const char& c)
	{
		std::string s;
		s.push_back(c);
		return s;
	}

	std::string toStr(const std::string& s)
	{
		return s;
	}

	template<class T>
	std::string toStr(const T& arg)
	{
		std::string s = std::to_string(arg);
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