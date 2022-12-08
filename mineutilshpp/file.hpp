#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include"str.hpp"
#include"exception.hpp"

namespace mineutils
{
	/*	ios::app�������� //��׷�ӵķ�ʽ���ļ�
		ios::ate�������� //�ļ��򿪺�λ���ļ�β��ios:app�Ͱ����д�����
		ios::binary���� //�Զ����Ʒ�ʽ���ļ���ȱʡ�ķ�ʽ���ı���ʽ�����ַ�ʽ�������ǰ��
		ios::in��������  //�ļ������뷽ʽ�򿪣��ļ��������뵽�ڴ棩
		ios::out�������� //�ļ��������ʽ�򿪣��ڴ�����������ļ���
		ios::nocreate�� //�������ļ��������ļ�������ʱ��ʧ��
		ios::noreplace��//�������ļ������Դ��ļ�ʱ����ļ�����ʧ��
		ios::trunc����  //����ļ����ڣ����ļ�������Ϊ0
	*/
	class IniFile
	{
	public:
		IniFile(const std::string& path) :file_path(path)
		{

		}
		~IniFile()
		{
			file.close();
			content.clear();
		}
		IniFile(const IniFile& file) = delete;
		IniFile& operator=(const IniFile& file) = delete;

		//��ini�ļ�
		bool open()   
		{
			if (file.is_open())
			{
				printf("IniFile: �ļ��ѱ���");
				file.close();
			}
			file.open(file_path, std::ios::binary | std::ios::in);
			if (!file.is_open())
			{
				std::cout << makeMessageW("IniFile", __func__, "�ļ���ʧ�ܣ������ļ��Ƿ�ռ�û�·���Ƿ���ڣ�\n");
				return false;
			}
			else
			{
				std::string line;
				content.clear();
				while (file.good())   //���ж�ȡ���ݣ���ȥ��\r��\n����
				{
					line.clear();
					std::getline(file, line);
					if (line.size() > 0)
					{
						line = split(line, "\n", false)[0];
						line = split(line, "\r", false)[0];
					}
					content.push_back(line);
				}
				return true;
			}
		}

		//�رղ�����
		void close()
		{
			if (rwstatus == 'w')
				saveContent();
			content.clear();
			file.close();
			rwstatus = 'r';
		}

		std::string getValue(const std::string& section, const std::string& key)
		{
			std::string value;
			int section_start = findSection(section);
			if (section_start >= 0)
			{
				int key_idx = findKey(section_start, key);
				if (key_idx >= 0)
				{
					value = split(content[key_idx], "=")[1];
					value = split(value, " ")[0];
					for (const std::string& sign : note_signs)
						value = split(value, sign)[0];
				}
				else
				{
					std::cout << makeMessageW("IniFile", __func__, "key=" + key + "�����ڣ�\n");
				}
			}
			else
			{
				std::cout << makeMessageW("IniFile", __func__, "section=" + section + "�����ڣ�\n");
			}
			file.seekg(0);
			return value;
		}

		void setValue(const std::string& section, const std::string& key, const std::string& value)
		{
			rwstatus = 'w';
			int section_start = findSection(section);
			if (section_start >= 0)
			{
				int key_idx = findKey(section_start, key);
				if (key_idx >= 0)
				{
					//std::string note = split(content[key_idx], ";")[1];
					content[key_idx] = key + "=" + value;// +"   " + note;
				}
				else
				{
					int section_end = findSectionEnd(section_start);
					content.insert(content.begin() + section_end, key + "=" + value);
				}
			}
			else
			{
				content.push_back("[" + section + "]");
				content.push_back(key + "=" + value);
			}
		}

	private:
		std::string file_path;
		std::fstream file;
		char rwstatus = 'r';
		std::vector<std::string> content;
		const std::vector<std::string> note_signs = {";", "#", "//"};

		/*����section����λ�ã�δ�ҵ��򷵻�-1*/
		int findSection(const std::string& section) const
		{
			for (int i = 0; i < content.size(); ++i)
			{
				if (content[i] == "[" + section + "]")
					return i;
			}
			return -1;
		}

		/*����key����λ�ã�δ�ҵ��򷵻�-1*/
		int findKey(int& section_start, const std::string& key) const
		{
			if (section_start >= 0)
			{
				for (int i = section_start + 1; i < content.size(); ++i)
				{
					if (content[i].size() > 0)
					{
						if (content[i][0] == '[')
							break;
						else if (split(content[i], "=")[0] == key)
							return i;
					}
				}
			}
			return -1;
		}

		///*����key����λ�ã�δ�ҵ��򷵻�-1*/
		//int findKey(const std::string& section, const std::string& key) const
		//{
		//	int section_start = findSection(section);
		//	if (section_start >= 0)
		//		return findKey(section_start, key);
		//	else return -1;
		//}

		/*����Section�����һ�е���һ������λ�ã�δ�ҵ��򷵻�-1*/
		int findSectionEnd(const int& section_start) const
		{
			for (int i = section_start + 1; i < content.size(); ++i)
			{
				if (content[i].size() > 0)
				{
					if (content[i][0] == '[')
						return i;
					else if (i == content.size() - 1)
						return content.size();
				}
				else for (int j = i + 1; j < content.size(); ++j)
				{
					if (content[j].size() > 0)
						if (content[j][0] == '[' or j == content.size() - 1)
							return i;	
				}
			}
			return -1;
		}

		/*����Section�����һ�е���һ������λ�ã�δ�ҵ��򷵻�-1*/
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
			for (std::string& line : content)
			{
				file << line << std::endl;
			}
		}
	};
}
