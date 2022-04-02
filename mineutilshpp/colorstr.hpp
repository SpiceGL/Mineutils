//base
#pragma once
#include <string>

using std::string;

class ColorStr
{
private:
	ColorStr()
	{}
	
public:
	static bool enabled;

public:
	
	template<class ...Strs>
	static string black(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[30m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string blue(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[34m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string cyan(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[36m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string fuchsia(string str, Strs ...strs)  //??¨°magenta
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[35m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string green(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[32m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string red(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[31m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string white(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[37m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

	template<class ...Strs>
	static string yellow(string str, Strs ...strs)
	{
		string color_str;
		if (ColorStr::enabled)
			color_str = "\033[33m" + ColorStr::_cat(str, strs...) + "\033[0m";
		else
			color_str = ColorStr::_cat(str, strs...);
		return color_str;
	}

private:
	template<class ...Strs>
	static string _cat(string str, Strs ...strs)
	{
		if constexpr (sizeof...(strs) > 0)
			return str + ColorStr::_cat(strs...);
		else
			return str;
	}
};

bool ColorStr::enabled = false;
