#pragma once
#include<iostream>
#include<set>
#include<string>
#include<vector>

#if defined(_MSC_VER)
//#if defined(WIN32) or defined(_WIN32) or defined(__WIN32) and !defined(__CYGWIN__)
#include<direct.h>
#include<io.h>
#else
#include<dirent.h>
#endif
#include<stdlib.h>
#include<sys/stat.h>

#include"str.hpp"
#include"exception.hpp"


namespace mineutils
{
	struct Path
	{
		/*
			封装一些常用的路径相关操作。
			-以'/'等分隔符结尾的路径会忽略末尾'/'号
		*/

		//判断路径是否存在
		static bool exists(std::string pth)   
		{
			pth = Path::normPath(pth);
			struct stat buffer;
			return stat(pth.c_str(), &buffer) == 0;
		}

		//获取输入的后缀名
		static std::string extension(std::string pth)   
		{
			std::string name = Path::splitName(pth);
			if (name.find(".") == -1)
				return "";
			else return name.substr(name.find_last_of('.') + 1);
		}

		//判断路径是否为绝对路径
		static bool isAbs(std::string pth)  
		{
			pth = Path::normPath(pth);
			if (pth.find(":/") != -1)
				return pth.substr(1, 2) == ":/";
			else return pth.substr(0, 1) == "/";
		}

		//判断路径是否为目录
		static bool isDir(std::string pth)  
		{
			pth = Path::normPath(pth);
			struct stat buffer;
			if (stat(pth.c_str(), &buffer) == 0)
			{
				if (buffer.st_mode & S_IFDIR)
					return true;
			}
			return false;
		}

		//判断路径是否为文件
		static bool isFile(std::string pth)  
		{
			pth = Path::normPath(pth);
			struct stat buffer;
			if (stat(pth.c_str(), &buffer) == 0)
			{
				if (buffer.st_mode & S_IFREG)
					return true;
			}
			return false;
		}

		//实现类似python的os.path.join功能
		template<class... Strs>
		static std::string join(std::string pth1, std::string pth2, Strs... pths)  
		{
			pth1 = Path::normPath(pth1);
			return Path::normPath(Path::_join(pth1, pth2, pths...));
		}

		template<class... Strs>
		static std::string _join(std::string pth1, std::string pth2, Strs... pths)
		{
			pth2 = Path::normPath(pth2);
			std::string pth = pth1 + "/" + pth2;
			return Path::_join(pth, pths...);
		}

		static std::string _join(std::string pth)
		{
			return pth;
		}

#if defined(_MSC_VER)
//#if defined(WIN32) or defined(_WIN32) or defined(__WIN32) and !defined(__CYGWIN__)
		static std::vector<std::string> listDir(std::string pth, bool return_path = true, std::set<std::string> ignore_names = {})  //获取目录下的一级文件和目录
		{
			pth = Path::normPath(pth);
			intptr_t hFile = 0;
			struct _finddata_t fileinfo;
			std::vector<std::string> files;
			std::string p = pth;

			if (not Path::isDir(pth))
			{
				std::cout << makeMessageW("Path", __func__, "输入路径不是目录或不存在，已返回空结果！") << std::endl;
				return {};
			}
			if ((hFile = _findfirst(p.append("/*").c_str(), &fileinfo)) != -1) 
			{
				do 
				{
					std::string fname = fileinfo.name;
					bool ignore = (ignore_names.find(fname) != ignore_names.end());

					if (!ignore and fname != "." and fname != "..")
					{
						// 保存文件的全路径

						if (return_path)
							files.push_back(Path::join(pth, fname));
						else files.push_back(fname);
					}
				} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
				_findclose(hFile);
			}
			return files;
		}

		static bool makeDirs(std::string pth)  //创建目录
		{
			if (Path::exists(pth))
				return true;
			pth = Path::normPath(pth);
			size_t sep_pos;
			std::string pth_tree;
			if (Path::isAbs(pth))
				sep_pos = 2;
			else sep_pos = 0;
			bool mk_res = true;
			do
			{
				sep_pos = pth.find("/", sep_pos + 1);
				pth_tree = pth.substr(0, sep_pos);
				if (not Path::isDir(pth_tree))
					_mkdir(pth_tree.c_str());
			} while (sep_pos != -1);
			return Path::isDir(pth);
		}
#else
		static std::vector<std::string> listDir(std::string pth, bool return_path = true, std::set<std::string> ignore_names = {})
		{
			DIR* pDir;
			struct dirent* ptr;
			std::vector<std::string> files;

			if (not Path::isDir(pth))
			{
				std::cout << makeMessageW("Path", __func__, "输入路径不是目录或不存在，已返回空结果！") << std::endl;
				return {};
			}
			pDir = opendir(pth.c_str());
			while ((ptr = readdir(pDir)) != nullptr) 
			{
				std::string fname = ptr->d_name;
				bool ignore = (ignore_names.find(fname) != ignore_names.end());
				if (!ignore and fname != "." and fname != "..")
				{
					if (return_path)
						files.push_back(Path::join(pth, fname));
					else files.push_back(fname);
				}
			}
			closedir(pDir);
			return files;
		}

		static bool makeDirs(std::string pth)  //创建目录
		{
			if (Path::exists(pth))
				return true;
			pth = Path::normPath(pth);
			size_t sep_pos = 0;
			std::string pth_tree;
			do
			{
				sep_pos = pth.find("/", sep_pos + 1);
				pth_tree = pth.substr(0, sep_pos);
				if (not Path::isDir(pth_tree))
					mkdir(pth_tree.c_str(), 0777);
			} while (sep_pos != -1);
			return Path::isDir(pth);
		}
#endif

		static std::string normPath(std::string pth)   //将windows路径中的\\变为标准的/分隔符
		{
			//std::string::find只有完全匹配才返回正确idx，std::string::find_first_of在有一个字符匹配时就返回正确idx
			size_t pos;
			while (pth.find("\\") != -1)
			{
				pos = pth.find("\\");
				pth.replace(pos, 1, "/");
			}
			while (pth.find("//") != -1)
			{
				pos = pth.find("//");
				pth.replace(pos, 2, "/");
			}
			while (pth.find("/./") != -1)
			{
				pos = pth.find("/./");
				pth.replace(pos, 3, "/");
			}
			if (pth.find("./") == 0)
			{
				pth.replace(0, 2, "");
			}
			if (pth.rfind("/.") != -1 and pth.rfind("/.") == pth.size() - 2)
			{
				pos = pth.rfind("/.");
				pth.replace(pos + 1, 1, "");
			}
			if (pth.rfind("/") == pth.size() - 1)
			{
				if (pth.find(":/") == 1 and pth.size() > 4)
					pth.replace(pth.size() - 1, 1, "");
				else if (pth.find(":/") == -1 and pth.size() > 1)
					pth.replace(pth.size() - 1, 1, "");
			}
			return pth;
		}

		static std::string parent(std::string pth)
		{
			pth = Path::normPath(pth);
			std::string parent = pth.substr(0, pth.find_last_of('/'));
			return parent;
		}

		static std::string splitName(std::string pth, bool suffix = true)  //从路径字符串获取文件名
		{
			pth = Path::normPath(pth);
			std::string name = pth.substr(pth.find_last_of('/') + 1);  
			if (not suffix)
				name = name.substr(0, name.find_last_of("."));
			return name;
		}

		static std::vector<std::string> walk(std::string pth, bool return_path = true)  //遍历目录下的所有文件
		{
			std::vector<std::string> filenames;
			std::string filename;

			if (not Path::isDir(pth))
			{
				std::cout << makeMessageW("Path", __func__, "输入路径不是目录或不存在，已返回空结果！") << std::endl;
				return {};
			}
			std::vector<std::string> listdir_res = Path::listDir(pth, true);
			for (std::string& f_d_path : listdir_res)
			{
				if (Path::isDir(f_d_path))
				{
					std::vector<std::string> sub_filenames = Path::walk(f_d_path, return_path);
					filenames.insert(filenames.end(), sub_filenames.begin(), sub_filenames.end());
				}
				else return_path ? filenames.push_back(f_d_path) : filenames.push_back(Path::splitName(f_d_path));
			}
			return filenames;
		}	
	};
}