#pragma once
#include<chrono>
#include<thread>


/*基于<chrono>库的简易时间函数封装*/
namespace ltime
{
	/*当前时间(纳秒)*/
	inline std::chrono::high_resolution_clock::time_point now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	/*封装“时间段(秒)”类*/
	inline std::chrono::seconds s(const int& t)
	{
		return std::chrono::seconds(t);
	}

	/*封装“时间段(毫秒)”类*/
	inline std::chrono::milliseconds ms(const int& t)
	{
		return std::chrono::milliseconds(t);
	}

	/*封装“时间段(微秒)”类*/
	inline std::chrono::microseconds us(const int& t)
	{
		return std::chrono::microseconds(t);
	}

	/*封装“时间段(纳秒)”类*/
	inline std::chrono::nanoseconds ns(const int& t)
	{
		return std::chrono::nanoseconds(t);
	}

	/*将“时间段(纳秒)”类型转化为以秒为单位的数字*/
	inline long long count(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(t).count();
	}
	
	/*将“时间段(纳秒)”类型转化为以毫秒为单位的数字*/
	inline long long mcount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	}

	/*将“时间段(纳秒)”类型转化为以微秒为单位的数字*/
	inline long long ucount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	}

	/*将“时间段(纳秒)”类型转化为以纳秒为单位的数字*/
	inline long long ncount(const std::chrono::nanoseconds& t)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
	}

	/*进程休眠(秒)*/
	inline void sleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::seconds(t));
	}

	/*进程休眠(毫秒)*/
	inline void msleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(t));
	}

	/*进程休眠(微秒)*/
	inline void usleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(t));
	}

	/*进程休眠(纳秒)*/
	inline void nsleep(const int& t)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(t));
	}
};
