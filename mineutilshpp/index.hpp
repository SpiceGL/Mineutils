#pragma once
#include"print.hpp"
#include"sign.hpp"

using std::pair;

template<class T>
pair<int, int> normRange(const T& range, int len)
{
	using cs = ColorStr;
	int dst_start, dst_end;
	if (isInTypes<T, int, pair<int, int>, sign::All>())
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

		else if constexpr (isSameType<T, pair<int, int>>())
		{
			int x1 = range.first, x2 = range.second;

			if (x1 >= 0)
				x1 < len ? dst_start = x1 : dst_start = len;
			else x1 >= -len ? dst_start = x1 + len : dst_start = 0;

			if (x2 >= 0)
				x2 < len ? dst_end = x2 : dst_end = len;
			else x2 >= -len ? dst_end = x2 + len : dst_end = 0;
		}

		else dst_start = 0, dst_end = len;
	}
	else
	{
		print(cs::red(__func__, ":"), "range输入类型有误，程序已终止！");
		exit(0);
	}
	return { dst_start, dst_end };
}
