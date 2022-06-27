#pragma once
#include<algorithm>
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
	class ColorStr
	{
	private:
		ColorStr() {}
	public:
		static bool enabled;

		template<class ...Strs>
		static string black(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[30m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string blue(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[34m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string cyan(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[36m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string fuchsia(string str, Strs ...strs)  //??òmagenta
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[35m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string green(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[32m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string red(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[31m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string white(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
				color_str = "\033[37m" + ColorStr::_cat(str, strs...) + "\033[0m";
			else color_str = ColorStr::_cat(str, strs...);
			return color_str;
		}

		template<class ...Strs>
		static string yellow(string str, Strs ...strs)
		{
			string color_str;
			if (ColorStr::enabled)
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
	bool ColorStr::enabled = false;


	template<class T>
	string toStr(const T& arg)
	{
		std::ostringstream buffer;
		buffer << arg;
		return buffer.str();
	}
	
	string zfillInt(int n, int len = 0)   //输入int值，返回len长度的字符串
	{
		string s = std::to_string(n);
		if (s.length() < len)
		{
			s = string(len - s.length(), '0') + s;
		}
		return s;
	}

	string zfillStr(string s, int len = 0)
	{
		if (s.length() < len)
		{
			s = string(len - s.length(), ' ') + s;
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
			cout << cs::red(__func__, ":") << " fstring待替换参数量过少，程序已中止！" << endl;
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
			string arg_s = toStr(arg);
			s.replace(pos, 2, arg_s);
		}
		else
		{
			cout << cs::red(__func__, ":") << "fstring待替换参数量过多，程序已中止！" << endl;
			exit(0);
		}
		return _fstr(s, args...);
	}

	template<class... Ts>
	string fstr(string s, Ts... args)   //实现类似于python的f-string功能，将字符串中的{}替换为后面的参数
	{
		return _fstr(s, args...);	
	}
	

	vector<string> split(string s, string sep=" ", bool ignore_emptystr=true)
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

