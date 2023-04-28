#pragma once
//#pragma warning (disable: 4996)
#include<chrono>
#include<thread>


/*基于<chrono>库的简易计时函数封装*/
namespace litetime
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using Duration = decltype(TimePoint() - TimePoint());

	/*当前时间点(litetime::time_point)*/
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

	/*将“时间段(litetime::duration)”类型转化为以秒为单位的数字*/
	inline long long s(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(t).count();
	}

	/*将“时间段(litetime::duration)”类型转化为以毫秒为单位的数字*/
	inline long long ms(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	}

	/*将“时间段(litetime::duration)”类型转化为以微秒为单位的数字*/
	inline long long us(const Duration& t)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	}

	/*将“时间段(litetime::duration)”类型转化为以纳秒为单位的数字*/
	inline long long ns(const Duration& t)
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

	enum class Unit
	{
		s = 0,
		ms = 1,
		us = 2,
		ns = 3
	};

	/*	@brief 统计一定循环次数中某一代码段的时间消耗*/
	class MeanTimeCost
	{
	private:
		int m_now_statistical_times = 0;
		int m_target_count_times;
		int m_addstart_times = 0;
		int m_addend_times = 0;

		litetime::Duration m_time_cost = Duration(0);
		litetime::TimePoint m_start_t;
		litetime::TimePoint m_end_t;

		/*	@brief 判断是否完成目标统计次数*/
		bool finish()
		{
			if (m_now_statistical_times >= m_target_count_times)
				return true;
			else return false;
		}

		/*	@brief 完成目标统计次数并打印统计结果后，重置内部统计计数*/
		void restart()
		{
			m_now_statistical_times = 0;
			m_addstart_times = 0;
			m_addend_times = 0;
			m_time_cost = Duration(0);
		}

	public:
		/*	@brief 构造MeanTimeCost类*/
		MeanTimeCost()
		{
			m_target_count_times = 1;
		}

		/*	@brief 构造MeanTimeCost类
		*	@param target_count_times: 目标统计次数，调用MeanTimeCost::printMeanTimeCost方法后在达到目标次数时会输出平均消耗时间 */
		MeanTimeCost(int target_count_times)
		{
			m_target_count_times = target_count_times;
		}

		/*	@brief 本轮统计开始，应在目标统计代码段前调用，与段后addEnd成对出现 */
		void addStart()
		{
			m_start_t = litetime::now();
			m_addstart_times += 1;
			assert(m_addend_times + 1 == m_addstart_times);
		}

		/*	@brief 本轮统计结束，应在目标统计代码段后调用，与段前addStart成对出现 */
		void addEnd()
		{
			m_end_t = litetime::now();
			m_time_cost += (m_end_t - m_start_t);
			m_addend_times += 1;
			assert(m_addend_times == m_addstart_times);
			m_now_statistical_times += 1;
		}

		/*	@brief 在达到目标统计次数后输出平均消耗时间，并重新开始统计时间
		*	@param print_head: 输出信息的头部内容，推荐输入调用printMeanTimeCost的函数的名字
		*	@param code_para_key: 输出信息中对被统计代码段的称呼(key)
		*	@param time_unit: 输出信息中时间统计的单位，输入强枚举类型litetime::Unit的成员
		*	@return 若达到目标统计次数，则按time_unit返回平均耗时；否则返回-1 */
		long long printMeanTimeCost(const std::string& print_head, const std::string& code_para_key, litetime::Unit time_unit = litetime::Unit::ms)
		{
			if (this->finish())
			{
				long long mean_time_cost;
				if (time_unit == litetime::Unit::s)
				{
					mean_time_cost = litetime::s(m_time_cost) / m_now_statistical_times;
					printf("%s: %s mean cost time %llds in %d counts\n", print_head.c_str(), code_para_key.c_str(), mean_time_cost, m_now_statistical_times);
				}
				else if (time_unit == litetime::Unit::ms)
				{
					mean_time_cost = litetime::ms(m_time_cost) / m_now_statistical_times;
					printf("%s: %s mean cost time %lldms in %d counts\n", print_head.c_str(), code_para_key.c_str(), mean_time_cost, m_now_statistical_times);
				}
				else if (time_unit == litetime::Unit::us)
				{
					mean_time_cost = litetime::us(m_time_cost) / m_now_statistical_times;
					printf("%s: %s mean cost time %lldus in %d counts\n", print_head.c_str(), code_para_key.c_str(), mean_time_cost, m_now_statistical_times);
				}
				else if (time_unit == litetime::Unit::ns)
				{
					mean_time_cost = litetime::ns(m_time_cost) / m_now_statistical_times;
					printf("%s: %s mean cost time %lldns in %d counts\n", print_head.c_str(), code_para_key.c_str(), mean_time_cost, m_now_statistical_times);
				}
				else
				{
					mean_time_cost = litetime::ms(m_time_cost) / m_now_statistical_times;
					printf("%s: %s mean cost time %lldms in %d counts\n", print_head.c_str(), code_para_key.c_str(), mean_time_cost, m_now_statistical_times);
				}
				this->restart();
				return mean_time_cost;
			}
			else return -1;
		}
	};

	/*	@brief 用于分别统计多个代码段的在一定循环次数的平均消耗时间。调用addStart和addEnd会带来少量时间损耗(在rv1126上约为12微秒)*/
	class MoreMeanTimeCost
	{
	private:
		int m_target_count_times;
		std::map<std::string, MeanTimeCost> m_time_counter;
		std::vector<std::string> m_keys;

	public:
		/*	@brief 构造MoreMeanTimeCost类
		*	@param target_count_times: 目标统计次数 */
		MoreMeanTimeCost(int target_count_times)
		{
			m_target_count_times = target_count_times;
		}

		/*	@brief 本轮统计开始，应在目标统计代码段前调用，与段后addEnd成对出现
		*	@param code_para_key: 要统计的代码段称呼(key) */
		void addStart(const std::string& code_para_key)
		{
			if (m_time_counter.end() == m_time_counter.find(code_para_key))
			{
				m_time_counter[code_para_key] = MeanTimeCost(m_target_count_times);
				m_keys.push_back(code_para_key);
			}
			m_time_counter[code_para_key].addStart();
		}

		/*	@brief 本轮统计结束，应在目标统计代码段后调用，与段前addStart成对出现
		*	@param code_para_key: 要统计的代码段称呼(key) */
		void addEnd(const std::string& code_para_key)
		{
			if (m_time_counter.end() == m_time_counter.find(code_para_key))
			{
				printf("!!!Error!!! MoreMeanTimeCost::%s: Please call \"addStart(%s)\" before \"addEnd(%s)\"!\n", __func__, code_para_key.c_str(), code_para_key.c_str());
				return;
			}
			m_time_counter[code_para_key].addEnd();
		}

		/*	@brief 在code_para_key代码段达到目标统计次数后输出平均消耗时间，并重新开始统计此段代码
		*	@param print_head: 输出信息的头部内容，推荐输入调用printMeanTimeCost的函数的名字
		*	@param code_para_key: 输出信息中对被统计代码段的称呼(key)
		*	@param time_unit: 输出信息中时间统计的单位，输入强枚举类型litetime::Unit的成员
		*	@return 若达到目标统计次数，则按time_unit返回平均耗时；否则返回-1 */
		long long printMeanTimeCost(const std::string& print_head, const std::string& code_para_key, litetime::Unit time_unit = litetime::Unit::ms)
		{
			return m_time_counter[code_para_key].printMeanTimeCost(print_head, code_para_key, time_unit);
		}

		/*	@brief 在每个被统计的代码段达到目标统计次数后，输出其平均消耗时间并重新开始统计此段代码
		*	@param print_head: 输出信息的头部内容，推荐输入调用printAllMeanTimeCost的函数的名字
		*	@param time_unit: 输出信息中时间统计的单位，输入强枚举类型litetime::Unit的成员 */
		void printAllMeanTimeCost(const std::string& print_head, litetime::Unit time_unit = litetime::Unit::ms)
		{
			for (const std::string& code_para_key : m_keys)
			{
				m_time_counter[code_para_key].printMeanTimeCost(print_head, code_para_key, time_unit);
			}
		}
	};
};

namespace ltime = litetime;