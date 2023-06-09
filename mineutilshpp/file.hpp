﻿#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include"str.hpp"
#include"exception.hpp"

namespace mineutils
{
	/*	ios::app：　　　 //以追加的方式打开文件
		ios::ate：　　　 //文件打开后定位到文件尾，ios:app就包含有此属性
		ios::binary：　 //以二进制方式打开文件，缺省的方式是文本方式。两种方式的区别见前文
		ios::in：　　　  //文件以输入方式打开（文件数据输入到内存）
		ios::out：　　　 //文件以输出方式打开（内存数据输出到文件）
		ios::nocreate： //不建立文件，所以文件不存在时打开失败
		ios::noreplace：//不覆盖文件，所以打开文件时如果文件存在失败
		ios::trunc：　  //如果文件存在，把文件长度设为0
	*/

	/*	@file file.hpp
	 *	@brief 读写ini文件。ini文件规范：#或//表示注释；内容为section-key-value的结构或key-value结构；section不可重复不可分段，同一section的key不可重复。
	*/
	class IniFile
	{
	public:
		IniFile()
		{

		}
		~IniFile()
		{
			this->close();
		}
		IniFile(const IniFile& file) = delete;
		IniFile& operator=(const IniFile& file) = delete;

		//打开ini文件
		bool open(const std::string& path)
		{
			file_path = path;
			if (file.is_open())
			{
				printf("IniFile: 文件已被打开");
				file.close();
			}
			file.open(file_path, std::ios::binary | std::ios::in);
			if (!file.is_open())
			{
				std::cout << makeMessageW("IniFile", __func__, "文件打开失败，请检查文件是否被占用或路径是否存在！\n");
				return false;
			}
			else
			{
				std::string line;
				content.clear();
				while (file.good())   //按行读取内容，并去掉\r和\n符号
				{
					line.clear();
					std::getline(file, line);
					if (line.size() > 0)
					{
						line = split(line, "\n")[0];
						line = split(line, "\r")[0];
					}
					content.push_back(line);
				}
				return true;
			}
		}

		//关闭并保存
		void close()
		{
			if (rwstatus == 'w')
				saveContent();
			content.clear();
			file.close();
			rwstatus = 'r';
			file.clear();
		}

		//根据section和key获取值
		std::string getValue(const std::string& section, const std::string& key)
		{
			std::string value;
			int section_start;
			if (section.size() > 0)
				section_start = findSection(section);
			else section_start = 0;
			if (section_start >= 0)
			{
				int key_idx = findKey(section_start, key);
				if (key_idx >= 0)
				{
					value = split(content[key_idx], "=")[1];
					value = split(value)[0];
					for (const std::string& sign : note_signs)
						value = split(value, sign)[0];
				}
				else
				{
					std::cout << makeMessageW("IniFile", __func__, "key=" + key + "不存在！\n");
				}
			}
			else
			{
				std::cout << makeMessageW("IniFile", __func__, "section=" + section + "不存在！\n");
			}
			file.seekg(0);
			return value;
		}

		//根据key获得值，没有section时使用
		std::string getValue(const std::string& key)
		{
			return getValue("", key);
		}


		/*	@brief 根据section和key设置值
		*	@param value: int、float等数字类型或char、string等字符类型
		*/
		template<class T>
		void setValue(const std::string& section, const std::string& key, const T& value)
		{
			std::string s_value = toStr(value);
			rwstatus = 'w';
			int section_start;
			if (section.size() > 0)
				section_start = findSection(section);
			else section_start = 0;
			if (section_start >= 0)
			{
				int key_idx = findKey(section_start, key);
				if (key_idx >= 0)
				{
					//std::string note = split(content[key_idx], ";")[1];
					content[key_idx] = key + "=" + s_value;// +"   " + note;
				}
				else
				{
					int section_end;
					if (section.size() > 0)
						section_end = findSectionEnd(section_start);
					else section_end = content.size();
					content.insert(content.begin() + section_end, key + "=" + s_value);
				}
			}
			else
			{
				content.push_back("[" + section + "]");
				content.push_back(key + "=" + s_value);
			}
		}

		/*	@brief 根据key设置值
		*	@param value: int、float等数字类型或char、string等字符类型，使用sstream转换为字符串
		*/
		template<class T>
		void setValue(const std::string& key, const T& value)
		{
			setValue("", key, value);
		}

	private:
		std::string file_path;
		std::fstream file;
		char rwstatus = 'r';
		std::vector<std::string> content;
		const std::vector<std::string> note_signs = {"#", "//"};

		/*查找section所在位置，未找到则返回-1*/
		int findSection(const std::string& section) const
		{
			for (int i = 0; i < content.size(); ++i)
			{
				if (content[i] == "[" + section + "]")
					return i;
			}
			return -1;
		}

		/*查找key所在位置，未找到则返回-1*/
		int findKey(int& section_start, const std::string& key) const
		{
			if (section_start >= 0)
			{
				for (int i = section_start + 1; i < content.size(); ++i)
				{
					if (content[i].size() > 0)
					{
						if (isSection(content[i]))
							break;
						else if (content[i].find("=") != -1 and split(split(content[i], "=")[0])[0] == key)
							return i;
					}
				}
			}
			return -1;
		}

		bool isSection(const std::string& str) const
		{
			if (str[0] == '[')
				return true;
			else return false;
		}

		///*查找key所在位置，未找到则返回-1*/
		//int findKey(const std::string& section, const std::string& key) const
		//{
		//	int section_start = findSection(section);
		//	if (section_start >= 0)
		//		return findKey(section_start, key);
		//	else return -1;
		//}

		/*查找Section的最后一行的下一行所在位置*/
		int findSectionEnd(const int& section_start) const
		{
			for (int i = section_start + 1; i < content.size(); ++i)
			{
				if (content[i].size() > 0)
				{
					if (content[i][0] == '[')
						return i;
				}
				//else for (int j = i + 1; j < content.size(); ++j)
				//{
				//	if (content[j].size() > 0)
				//	{
				//		if (content[j][0] == '[' or j == content.size() - 1)
				//			return i;
				//		else break;
				//	}
				//	else if (j == content.size() - 1)
				//		return i;
				//	
				//}
			}
			return content.size();
		}

		/*查找Section的最后一行的下一行所在位置，未找到则返回-1*/
		int findSectionEnd(const std::string& section) const
		{
			int section_start = findSection(section);
			if (section_start >= 0)
				return findSectionEnd(section_start);
			else return -1;
		}

		void saveContent()
		{
			file.close();
			file.open(file_path, std::ios::binary | std::ios::trunc | std::ios::out);
			for (int i = 0; i < content.size(); ++i)
			{
				if (i == content.size() - 1)
					file << content[i];
				else file << content[i] << std::endl;
			}
		}
	};
}
