#pragma once
#include<algorithm>
#include<iomanip>
#include<sstream>
#include<string>
#include<vector>
#include<stdlib.h>

namespace mineutils
{
	using std::cout;
	using std::endl;
	using std::string;
	using std::vector;
	/*---------------------------------声明-------------------------------------*/

	/*---------------------------------定义-------------------------------------*/
	static bool ColorStr_enabled = false;   //在main函数cpp中更改，就可以影响全局，否则不影响

	class ColorStr
	{
	private:
		ColorStr() {}
	public:
		template<class ...Strs>
		static string black(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[30m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string blue(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[34m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string cyan(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[36m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string fuchsia(string str, Strs ...strs)  //
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[35m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string green(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[32m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string red(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[31m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string white(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[37m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string yellow(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr_enabled)
				color_str = "\033[33m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

	private:
		template<class ...Strs>
		static string _cat(string str, Strs ...strs)
		{
			return str + ColorStr::_cat(strs...);
		}

		static string _cat(string str)
		{
			return str;
		}
	};


	template<class T>
	inline string toStr(const T& arg)
	{
		std::ostringstream buffer;
		buffer << arg;
		return buffer.str();
	}
	
	inline string zfillInt(int n, int min_len = 0, char padding = '0')   //输入int值，返回len长度的字符串
	{
		string s = std::to_string(n);
		if (s.length() < min_len)
		{
			s = string(min_len - s.length(), padding) + s;
		}
		return s;
	}

	inline string zfillFlt(float f, int min_len_int = 0, int min_len_flt = 4,
		size_t flt_precision = 8, char int_padding = ' ', char flt_padding = '0')
	{
		std::ostringstream buffer;
		buffer << std::setprecision(min_len_flt) << f;
		string s = buffer.str();

		/*找到输入小数的整数部分和小数部分，分别处理并合并*/
		string int_part, flt_part;
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
			int_part = string(min_len_int - int_part.length(), int_padding) + int_part;
		if (flt_part.length() < min_len_flt)
			flt_part = flt_part + string(min_len_flt - flt_part.length(), flt_padding);
		s = int_part + "." + flt_part;
		return s;
	}

	/*--------------------------------------------------------------------------*/

	inline string _fstr(string& s)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			cout << cs::red(__func__, ":") << " fstr待替换参数量过少，程序已中止！" << endl;
			exit(0);
		}
		return s;
	}
	
	template<class T, class... Ts>
	inline string _fstr(string& s, T& arg, Ts&... args)
	{
		using cs = ColorStr;
		size_t pos = s.find("{}");
		if (pos != -1)
		{
			string arg_s = toStr(arg);
			s.replace(pos, 2, arg_s);
		}
		else
		{
			cout << cs::red(__func__, ":") << "fstr待替换参数量过多，程序已中止！" << endl;
			exit(0);
		}
		return _fstr(s, args...);
	}

	template<class... Ts>
	inline string fstr(string s, Ts... args)   //实现类似于python的f-string功能，将字符串中的{}替换为后面的参数
	{
		return _fstr(s, args...);	
	}
	

	inline vector<string> split(string s, string sep=" ", bool ignore_emptystr=true)
	{
		vector<string> strs;
		size_t sep_pos;
		string s_in;   //s_in存放已处理的字段，s存放待处理的字段
		while ((sep_pos = s.find(sep)) != -1)
		{
			s_in = s.substr(0, sep_pos);
			s = s.substr(sep_pos + sep.length());
			if ((ignore_emptystr and s_in.length() != 0) or !ignore_emptystr)
				strs.push_back(s_in);
		}
		if ((ignore_emptystr and s.length() != 0) or !ignore_emptystr)
			strs.push_back(s);
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