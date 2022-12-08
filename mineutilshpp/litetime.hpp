#pragma once
//#pragma warning (disable: 4996)
#include<chrono>
#include<thread>


/*基于<chrono>库的简易计时函数封装*/
namespace litetime
{
	using time_point = std::chrono::high_resolution_clock::time_point;
	using duration = decltype(time_point() - time_point());

	/*当前时间点(litetime::time_point)*/
	inline time_point now()
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

	/*将“时间段(litetime::duration)”类型转化为以秒为单位的数字*/
	inline long long s(const duration& t)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(t).count();
	}
	
	/*将“时间段(litetime::duration)”类型转化为以毫秒为单位的数字*/
	inline long long ms(const duration& t)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	}

	/*将“时间段(litetime::duration)”类型转化为以微秒为单位的数字*/
	inline long long us(const duration& t)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	}

	/*将“时间段(litetime::duration)”类型转化为以纳秒为单位的数字*/
	inline long long ns(const duration& t)
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

namespace ltime = litetime;