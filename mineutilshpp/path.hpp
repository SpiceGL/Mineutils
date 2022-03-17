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
	static void copy(string pth_from, string pth_to, string opt = "overwrite")  //�ݹ鸴���ļ���Ŀ¼
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
			print(cs::yellow("Path::", __func__, ":"), cs::cyan("pth_from= " + pth_from) + "�����ڣ����δ�����κ�Ŀ¼���ļ���");
	}

	static void _copyDir(string pth_from, string pth_to)  //ֻ�ܸ���Ŀ¼��Ŀ¼�µ��ļ����ļ��ж��޷�����
	{
		fs::path pf(pth_from);
		fs::path pt(pth_to);
		fs::copy(pf, pt, fs::copy_options::skip_existing);
	}

	static bool _copyFile(string pth_from, string pth_to, string opt = "overwrite")  //���Ƶ����ļ����������ܴ���Ŀ¼
	{
		/*
		opt:
		"overwrite"��ʾ�����Ѵ��ڵ�Ŀ�ꣻ
		"skip"���������Ѵ��ڵ�Ŀ�ꣻ
		"update"��ʾ����pth_from����޸�ʱ������pth_to��ʱ�򸲸ǣ�����������
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
			throw "opt��������";
	}

	static string cwd()  //��ȡ��ǰ����·��
	{
		return fs::current_path().string();
	}

	static bool exists(string pth)  //�ж�·���Ƿ����
	{
		fs::path p(pth);
		return fs::exists(p);
	}

	static string extension(string pth)  //��ȡ·������չ��
	{
		fs::path p(pth);
		return p.extension().string();
	}

	static bool isAbs(string pth)  //�ж�·���Ƿ�Ϊ����·��
	{
		fs::path p(pth);
		return p.is_absolute();
	}

	static bool isDir(string pth)  //�ж�·���Ƿ�ΪĿ¼
	{
		fs::path p(pth);
		return fs::is_directory(p);
	}

	static bool isFile(string pth)  //�ж�·���Ƿ�Ϊ�ļ�
	{
		fs::path p(pth);
		return fs::exists(p) and not fs::is_directory(p);
	}

	template<class... Strs>
	static string join(string pth1, string pth2, Strs... pths)  //ʵ������python��os.path.join����
	{
		fs::path p(pth1);
		p /= pth2;
		if constexpr (sizeof...(pths) > 0)
			return Path::join(p.string(), pths...);
		else
			return p.string();
	}

	static vector<string> listDir(string pth, bool return_path = true, set<string> ignore_names = {})  //��ȡĿ¼�µ�һ���ļ���Ŀ¼
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

	static bool makeDirs(string pth)  //����Ŀ¼
	{
		fs::path p(pth);
		return fs::create_directories(p);
	}

	static void move(string pth_from, string pth_to)  //�ƶ�Ŀ¼��Ҳ������������
	{
		fs::path pf(pth_from);
		fs::path pt(pth_to);
		if (not fs::exists(pt.parent_path()))
			fs::create_directories(pt.parent_path());
		fs::rename(pf, pt);
	}

	static string parent(string pth)  //��ȡ��Ŀ¼
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

	static uintmax_t remove(string pth)  //�ݹ�ɾ��Ŀ¼���ļ�
	{
		fs::path p(pth);
		return fs::remove_all(p);
	}

	static string root(string pth)  //���ظ�Ŀ¼
	{
		fs::path p(pth);
		return p.root_path().string();
	}

	static string splitName(string pth, bool suffix = true)
		//��·���ַ�����ȡ�ļ���
	{
		fs::path p(pth);
		if (suffix)
			return p.filename().string();
		else
			return p.stem().string();
	}

	static vector<string> walk(string pth, bool return_path = true)  //����Ŀ¼�µ������ļ�
	{
		fs::path p(pth);
		vector<string> filenames;
		string filename;

		if (not fs::is_directory(p))
		{
			print(cs::red("Path::", __func__, ":"), "����·������Ŀ¼�򲻴��ڣ���������ֹ��");
			exit(0);
		}
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