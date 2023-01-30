#pragma once
#include<string>
#include"str.hpp"


namespace mineutils
{
	//生成警告信息
	inline std::string makeMessageW(const std::string& class_name, const std::string& func_name, const std::string& content)
	{
		std::string color_msg;
		if (class_name.size() == 0)
			color_msg = fstr("!Warning! {}: ", func_name); 
		else
			color_msg = fstr("!Warning! {}::{}: ", class_name, func_name);
		return cstr::yellow(color_msg) + content;
	}

	//生成警告信息
	inline std::string makeMessageW(const std::string& func_name, const std::string& content)
	{
		return makeMessageW("", func_name, content);
	}

	//生成错误信息
	inline std::string makeMessageE(const std::string& class_name, const std::string& func_name, const std::string& content)
	{
		std::string color_msg;
		if (class_name.size() == 0)
			color_msg = fstr("!!!Error!!! {}: ", func_name);
		else
			color_msg = fstr("!!!Error!!! {}::{}: ", class_name, func_name);
		return cstr::red(color_msg) + content;
	}

	//生成错误信息
	inline std::string makeMessageE(const std::string& func_name, const std::string& content)
	{
		return makeMessageE("", func_name, content);
	}

	//生成正常提示信息，在ColorStr_enable==true时可以选择颜色
	inline std::string makeMessageN(const std::string& class_name, const std::string& func_name, const std::string& content, const std::string& color = "cyan")
	{
		std::string color_msg;
		if (class_name.size() == 0)
			color_msg = fstr("{}: ", func_name);
		else
			color_msg = fstr("{}::{}: ", class_name, func_name);

		if (color == "black")
			return cstr::black(color_msg) + content;
		else if (color == "blue")
			return cstr::blue(color_msg) + content;
		else if (color == "cyan")
			return cstr::cyan(color_msg) + content;
		else if (color == "fuchsia")
			return cstr::fuchsia(color_msg) + content;
		else if (color == "green")
			return cstr::green(color_msg) + content;
		else if (color == "red")
			return cstr::red(color_msg) + content;
		else if (color == "white")
			return cstr::white(color_msg) + content;
		else if (color == "yellow")
			return cstr::yellow(color_msg) + content;
	}

	//生成正常提示信息，在ColorStr_enable==true时可以选择颜色
	inline std::string makeMessageN(const std::string& func_name, const std::string& content, const std::string& color = "cyan")
	{
		return makeMessageN("", func_name, content, color);
	}
}
