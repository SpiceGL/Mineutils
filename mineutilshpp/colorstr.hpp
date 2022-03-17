#pragma once
#include <string>

using std::string;

struct ColorStr
{
	static string black(string str)
	{
		string color_str = "\033[30m" + str + "\033[0m";
		return color_str;
	}

	static string blue(string str)
	{
		string color_str = "\033[34m" + str + "\033[0m";
		return color_str;
	}

	static string cyan(string str)
	{
		string color_str = "\033[36m" + str + "\033[0m";
		return color_str;
	}

	static string fuchsia(string str)  //或magenta
	{
		string color_str = "\033[35m" + str + "\033[0m";
		return color_str;
	}

	static string green(string str)
	{
		string color_str = "\033[32m" + str + "\033[0m";
		return color_str;
	}

	static string red(string str)
	{
		string color_str = "\033[31m" + str + "\033[0m";
		return color_str;
	}

	static string white(string str)
	{
		string color_str = "\033[37m" + str + "\033[0m";
		return color_str;
	}

	static string yellow(string str)
	{
		string color_str = "\033[33m" + str + "\033[0m";
		return color_str;
	}
};
