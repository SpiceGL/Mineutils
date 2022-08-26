#pragma once
#include<iostream>
#include<set>
#include<string>
#include<vector>

#if defined(WIN32) or defined(_WIN32) or defined(__WIN32) and !defined(__CYGWIN__)
#include<direct.h>
#include<io.h>
#else
#include<dirent.h>
#endif
#include<stdlib.h>
#include<sys/stat.h>

#include"str.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;
	using std::pair;
	using std::set;
	using std::string;
	using std::vector;


	struct Path
	{
		/*
		��װһЩ���õ�·����ز�����
		-��/�ȷָ�����β��·���ᱻ����/ǰ��Ŀ¼·��
		*/
		using cs = ColorStr;

		static bool exists(string pth)   //�ж�·���Ƿ����
		{
			pth = Path::normPath(pth);
			struct stat buffer;
			return stat(pth.c_str(), &buffer) == 0;
		}

		static string extension(string pth)   //��ȡ����ĺ�׺��
		{
			string name = Path::splitName(pth);
			if (name.find(".") == -1)
				return "";
			else return name.substr(name.find_last_of('.') + 1);
		}

		static bool isAbs(string pth)  //�ж�·���Ƿ�Ϊ����·��
		{
			pth = Path::normPath(pth);
			if (pth.find(":/") != -1)
				return pth.substr(1, 2) == ":/";
			else return pth.substr(0, 1) == "/";
		}

		static bool isDir(string pth)  //�ж�·���Ƿ�ΪĿ¼
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

		static bool isFile(string pth)  //�ж�·���Ƿ�Ϊ�ļ�
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

		template<class... Strs>
		static string join(string pth1, string pth2, Strs... pths)  //ʵ������python��os.path.join����
		{
			pth1 = Path::normPath(pth1);
			return Path::normPath(Path::_join(pth1, pth2, pths...));
		}

		template<class... Strs>
		static string _join(string pth1, string pth2, Strs... pths)
		{
			pth2 = Path::normPath(pth2);
			string pth = pth1 + "/" + pth2;
			return Path::_join(pth, pths...);
		}

		static string _join(string pth)
		{
			return pth;
		}

#if defined(WIN32) or defined(_WIN32) or defined(__WIN32) and !defined(__CYGWIN__)
		static vector<string> listDir(string pth, bool return_path = true, set<string> ignore_names = {})  //��ȡĿ¼�µ�һ���ļ���Ŀ¼
		{
			pth = Path::normPath(pth);
			intptr_t hFile = 0;
			struct _finddata_t fileinfo;
			vector<string> files;
			string p = pth;

			if (not Path::isDir(pth))
			{
				cout << cs::red("Path::", __func__, ":") << " ����·������Ŀ¼�򲻴��ڣ���������ֹ��" << endl;
				exit(0);
			}
			if ((hFile = _findfirst(p.append("/*").c_str(), &fileinfo)) != -1) 
			{
				do 
				{
					string fname = fileinfo.name;
					bool ignore = (ignore_names.find(fname) != ignore_names.end());

					if (!ignore and fname != "." and fname != "..")
					{
						// �����ļ���ȫ·��

						if (return_path)
							files.push_back(Path::join(pth, fname));
						else files.push_back(fname);
					}
				} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
				_findclose(hFile);
			}
			return files;
		}

		static bool makeDirs(string pth)  //����Ŀ¼
		{
			if (Path::exists(pth))
				return true;
			pth = Path::normPath(pth);
			size_t sep_pos;
			string pth_tree;
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
		static vector<string> listDir(string pth, bool return_path = true, set<string> ignore_names = {})
		{
			DIR* pDir;
			struct dirent* ptr;
			vector<string> files;

			if (not Path::isDir(pth))
			{
				cout << cs::red("Path::", __func__, ":") << " ����·������Ŀ¼�򲻴��ڣ���������ֹ��" << endl;
				exit(0);
			}
			pDir = opendir(pth.c_str());
			while ((ptr = readdir(pDir)) != nullptr) 
			{
				string fname = ptr->d_name;
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

		static bool makeDirs(string pth)  //����Ŀ¼
		{
			if (Path::exists(pth))
				return true;
			pth = Path::normPath(pth);
			size_t sep_pos = 0;
			string pth_tree;
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

		static string normPath(string pth)   //��windows·���е�\\��Ϊ��׼��/�ָ���
		{
			//string::findֻ����ȫƥ��ŷ�����ȷidx��string::find_first_of����һ���ַ�ƥ��ʱ�ͷ�����ȷidx
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

		static string parent(string pth)
		{
			pth = Path::normPath(pth);
			string parent = pth.substr(0, pth.find_last_of('/'));
			return parent;
		}

		static string splitName(string pth, bool suffix = true)  //��·���ַ�����ȡ�ļ���
		{
			pth = Path::normPath(pth);
			string name = pth.substr(pth.find_last_of('/') + 1);  
			if (not suffix)
				name = name.substr(0, name.find_last_of("."));
			return name;
		}

		static vector<string> walk(string pth, bool return_path = true)  //����Ŀ¼�µ������ļ�
		{
			vector<string> filenames;
			string filename;

			if (not Path::isDir(pth))
			{
				cout << cs::red("Path::", __func__, ":") << " ����·������Ŀ¼�򲻴��ڣ���������ֹ��" << endl;
				exit(0);
			}
			vector<string> listdir_res = Path::listDir(pth, return_path);
			for (string& f_d : listdir_res)
			{
				if (Path::isDir(f_d))
				{
					vector<string> sub_filenames = Path::walk(f_d, return_path);
					filenames.insert(filenames.end(), sub_filenames.begin(), sub_filenames.end());
				}
				else return_path ? filenames.push_back(f_d) : filenames.push_back(f_d);
			}
			return filenames;
		}	
	};
}