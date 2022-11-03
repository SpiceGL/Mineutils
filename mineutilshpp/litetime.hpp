#pragma once
//#pragma warning (disable: 4996)
#include<chrono>
#include<ctime>
#include<iomanip>
#include<sstream>
#include<string>
#include<thread>


/*����<chrono>��ļ��׼�ʱ������װ*/
namespace ltime
{
	/*��ǰʱ��(����)*/
	inline std::chrono::high_resolution_clock::time_point now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	//inline std::string getDateTime()
	//{
	//	std::chrono::system_clock::time_point tp = std::chrono::system_clock().now();
	//	time_t t = std::chrono::system_clock::to_time_t(tp);
	//	std::ostringstream st;
	//	st << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
	//	std::string dt = st.str();
	//	return dt;
	//}

	/*��װ��ʱ���(��)����*/
	inline std::chrono::seconds s(const int& t)
	{
		return std::chrono::seconds(t);
	}

	/*��װ��ʱ���(����)����*/
	inline std::chrono::milliseconds ms(const int& t)
	{
		return std::chrono::milliseconds(t);
	}

	/*��װ��ʱ���(΢��)����*/
	inline std::chrono::microseconds us(const int& t)
	{
		return std::chrono::microseconds(t);
	}

	/*��װ��ʱ���(����)����*/
	inline std::chrono::nanoseconds ns(const int& t)
	{
		return std::chrono::nanoseconds(t);
	}

	/*����ʱ���(����)������ת��Ϊ����Ϊ��λ������*/
	inline long long count(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(t).count();
	}
	
	/*����ʱ���(����)������ת��Ϊ�Ժ���Ϊ��λ������*/
	inline long long mcount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	}

	/*����ʱ���(����)������ת��Ϊ��΢��Ϊ��λ������*/
	inline long long ucount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	}

	/*����ʱ���(����)������ת��Ϊ������Ϊ��λ������*/
	inline long long ncount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
	}

	/*��������(��)*/
	inline void sleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::seconds(t));
	}

	/*��������(����)*/
	inline void msleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(t));
	}

	/*��������(΢��)*/
	inline void usleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(t));
	}

	/*��������(����)*/
	inline void nsleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(t));
	}
};