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

	pair<int, int> normRange(pair<int, int> range, int len);

	pair<int, int> normRange(int idx, int len);


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

	pair<int, int> normRange(pair<int, int> range, int len)
	{
		int dst_start, dst_end;
		int x1 = range.first, x2 = range.second;

		if (x1 >= 0)
			x1 < len ? dst_start = x1 : dst_start = len;
		else x1 >= -len ? dst_start = x1 + len : dst_start = 0;

		if (x2 >= 0)
			x2 < len ? dst_end = x2 : dst_end = len;
		else x2 >= -len ? dst_end = x2 + len : dst_end = 0;
		return { dst_start, dst_end };
	}

	pair<int, int> normRange(int idx, int len)
	{
		using cs = ColorStr;
		int dst_start, dst_end;
		if (idx >= 0 and idx < len)
			dst_start = idx, dst_end = idx + 1;
		else if (idx < 0 and idx >= -len)
			dst_start = idx + len, dst_end = idx + len + 1;
		else
		{
			print(cs::red(__func__, ":"), "idx=", idx, "超出索引范围，程序已中止！");
			exit(0);
		}
		return { dst_start, dst_end };
	}
}
