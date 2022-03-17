#pragma once
#include<filesystem>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<vector>


#include"print.hpp"
#include"colorstr.hpp"

namespace fs = std::filesystem;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using cs = ColorStr;


struct Path
{
	static void copy(string pth_from, string pth_to, string opt = "overwrite")  //递归复制文件和目录
	{
		if (Path::exists(pth_from))
		{
			if (Path::isDir(pth_from))
			{
				Path::_copyDir(pth_from, pth_to);
				auto paths = Path::listDir(pth_from, false);
				for (auto pth : paths)
					Path::copy(Path::join(pth_from, pth), Path::join(pth_to, pth), opt);
			}
			else
				Path::_copyFile(pth_from, pth_to, opt);
		}
		else
			print(cs::yellow("Path::copyDir:"), cs::cyan("pth_from= " + pth_from) + "不存在，因此未复制任何目录和文件！");
	}

	static void _copyDir(string pth_from, string pth_to)  //只能复制目录，目录下的文件和文件夹都无法复制
	{
		fs::path pf(pth_from);
		fs::path pt(pth_to);
		fs::copy(pf, pt, fs::copy_options::skip_existing);
	}

	static bool _copyFile(string pth_from, string pth_to, string opt = "overwrite")  //复制单个文件，甚至不能创建目录
	{
		/*
		opt:
		"overwrite"表示覆盖已存在的目标；
		"skip"代表跳过已存在的目标；
		"update"表示仅当pth_from最后修改时间新于pth_to的时候覆盖，否则跳过。
		*/
		fs::path pf(pth_from);
		fs::path pt(pth_to);

		if (opt == "overwrite")
			return fs::copy_file(pf, pt, fs::copy_options::overwrite_existing);
		else if (opt == "skip")
			return fs::copy_file(pf, pt, fs::copy_options::skip_existing);
		else if (opt == "update")
			return fs::copy_file(pf, pt, fs::copy_options::update_existing);
		else
			throw "opt输入有误！";
	}

	static string cwd()  //获取当前绝对路径
	{
		return fs::current_path().string();
	}

	static bool exists(string pth)  //判断路径是否存在
	{
		fs::path p(pth);
		return fs::exists(p);
	}

	static string extension(string pth)  //获取路径的扩展名
	{
		fs::path p(pth);
		return p.extension().string();
	}

	static bool isAbs(string pth)  //判断路径是否为绝对路径
	{
		fs::path p(pth);
		return p.is_absolute();
	}

	static bool isDir(string pth)  //判断路径是否为目录
	{
		fs::path p(pth);
		return fs::is_directory(p);
	}

	static bool isFile(string pth)  //判断路径是否为文件
	{
		fs::path p(pth);
		return fs::exists(p) and not fs::is_directory(p);
	}

	template<class... Strs>
	static string join(string pth1, string pth2, Strs... pths)  //实现类似python的os.path.join功能
	{
		fs::path p(pth1);
		p /= pth2;
		if constexpr (sizeof...(pths) > 0)
			return Path::join(p.string(), pths...);
		else
			return p.string();
	}

	static vector<string> listDir(string pth, bool return_path = true, set<string> ignore_names = {})  //获取目录下的一级文件和目录
	{
		fs::path p(pth);
		vector<string> filenames;
		string filename;

		assert(fs::is_directory(p));
		fs::directory_iterator iter(p);
		for (auto& it : iter)
		{
			string it_name = it.path().filename().string();
			auto find_res = ignore_names.find(it_name);
			if (find_res == ignore_names.end() and it_name != "..")
			{
				if (return_path)
					filename = Path::join(pth, it_name);
				else
					filename = it_name;
				filenames.push_back(filename);
			}
		}
		return filenames;
	}

	static bool makeDirs(string pth)  //创建目录
	{
		fs::path p(pth);
		return fs::create_directories(p);
	}

	static void move(string pth_from, string pth_to)  //移动目录，也可用于重命名
	{
		fs::path pf(pth_from);
		fs::path pt(pth_to);
		if (not fs::exists(pt.parent_path()))
			fs::create_directories(pt.parent_path());
		fs::rename(pf, pt);
	}

	static string parent(string pth)  //获取父目录
	{
		fs::path p(pth);
		if (p.is_absolute())
			return p.parent_path().string();
		else
		{
			if (p.filename().string() == "..")
			{
				p /= "..";
				return p.string();
			}
			else if (p.parent_path().string().size() == 0)
				return "..";
			else
				return p.parent_path().string();
		}
	}

	static uintmax_t remove(string pth)  //递归删除目录和文件
	{
		fs::path p(pth);
		return fs::remove_all(p);
	}

	static string root(string pth)  //返回根目录
	{
		fs::path p(pth);
		return p.root_path().string();
	}

	static string splitName(string pth, bool suffix = true)
		//从路径字符串获取文件名
	{
		fs::path p(pth);
		if (suffix)
			return p.filename().string();
		else
			return p.stem().string();
	}

	static vector<string> walk(string pth, bool return_path = true)  //遍历目录下的所有文件
	{
		fs::path p(pth);
		vector<string> filenames;
		string filename;

		assert(fs::is_directory(p));
		fs::directory_iterator iter(p);
		for (auto& it : iter)
		{	
			string it_name = it.path().filename().string();
			string it_path = Path::join(pth, it_name);
			if (it_name != "..")
			{
				if (Path::isDir(it_path))
				{
					vector<string> sub_filenames = Path::walk(it_path, return_path);
					filenames.insert(filenames.end(), sub_filenames.begin(), sub_filenames.end());
				}
				else
				{
					if (return_path)
						filenames.push_back(it_path);
					else
						filenames.push_back(it_name);
				}
			}
		}
		return filenames;
	}
};