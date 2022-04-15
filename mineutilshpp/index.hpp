#pragma once
#include<limits.h>
#include<stdlib.h>
#include"print.hpp"

using std::pair;

namespace mineutils
{
    /*------------------------------------声明-------------------------------------*/

	pair<int, int> allRange();

	int normIdx(int idx, int len);

	template<class T = pair<int, int>>
	pair<int, int> normRange(const T& range, int len);


	/*------------------------------------定义-------------------------------------*/
	pair<int, int> allRange()
	{
		return { 0, INT_MAX };
	}


	int normIdx(int idx, int len)
	{
		using cs = ColorStr;
		int normal_idx;
		if (idx >= 0 and idx < len)
			normal_idx = idx;
		else if (idx < 0 and idx >= -len)
			normal_idx = idx + len;
		else
		{
			print(cs::red(__func__, ":"), "idx=", idx, "超出索引范围，程序已中止！");
			exit(0);
		}
		return normal_idx;
	}


	template<class T>
	pair<int, int> normRange(const T& range, int len)
	{
		using cs = ColorStr;
		int dst_start, dst_end;
		if (isInTypes<T, int, pair<int, int>>())
		{
			if constexpr (isSameType<T, int>())
			{
				if (range >= 0 and range < len)
					dst_start = range, dst_end = range + 1;
				else if (range < 0 and range >= -len)
					dst_start = range + len, dst_end = range + len + 1;
				else
				{
					print(cs::red(__func__, ":"), "range=", range, "超出索引范围，程序已中止！");
					exit(0);
				}
			}
			else
			{
				int x1 = range.first, x2 = range.second;

				if (x1 >= 0)
					x1 < len ? dst_start = x1 : dst_start = len;
				else x1 >= -len ? dst_start = x1 + len : dst_start = 0;

				if (x2 >= 0)
					x2 < len ? dst_end = x2 : dst_end = len;
				else x2 >= -len ? dst_end = x2 + len : dst_end = 0;
			}
		}
		else
		{
			print(cs::red(__func__, ":"), "range输入类型有误，程序已终止！");
			exit(0);
		}
		return { dst_start, dst_end };
	}
}
