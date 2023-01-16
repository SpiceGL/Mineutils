#pragma once
//#pragma warning (disable: 4996)
#include<chrono>
#include<thread>


/*����<chrono>��ļ��׼�ʱ������װ*/
namespace litetime
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using Duration = decltype(TimePoint() - TimePoint());

	/*��ǰʱ���(litetime::time_point)*/
	inline TimePoint now()
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

	/*����ʱ���(litetime::duration)������ת��Ϊ����Ϊ��λ������*/
	inline long long s(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(t).count();
	}
	
	/*����ʱ���(litetime::duration)������ת��Ϊ�Ժ���Ϊ��λ������*/
	inline long long ms(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	}

	/*����ʱ���(litetime::duration)������ת��Ϊ��΢��Ϊ��λ������*/
	inline long long us(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	}

	/*����ʱ���(litetime::duration)������ת��Ϊ������Ϊ��λ������*/
	inline long long ns(const Duration& t)
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

namespace ltime = litetime;