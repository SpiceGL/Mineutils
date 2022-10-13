#pragma once
#include<limits.h>
#include<stdlib.h>
#include"str.hpp"


namespace mineutils
{
	using std::cout;
	using std::endl;
	using std::pair;
    /*------------------------------------声明-------------------------------------*/


	/*------------------------------------定义-------------------------------------*/
#define ALLIDX { 0, INT_MAX }
	
	

	inline int normIdx(int idx, int len)
	{
		using cs = ColorStr;
		int normal_idx;
		if (idx >= 0 and idx < len)
			normal_idx = idx;
		else if (idx < 0 and idx >= -len)
			normal_idx = idx + len;
		else
		{
			cout << cs::red(fstr("!!!Error!!! {}: ", __func__)) << fstr("idx={}超出索引范围！", idx) << endl;
			return -1;
		}
		return normal_idx;
	}

	inline pair<int, int> normRange(pair<int, int> range, int len)
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

	inline pair<int, int> normRange(int idx, int len)
	{
		using cs = ColorStr;
		int dst_start, dst_end;
		if (idx >= 0 and idx < len)
			dst_start = idx, dst_end = idx + 1;
		else if (idx < 0 and idx >= -len)
			dst_start = idx + len, dst_end = idx + len + 1;
		else
		{
			cout << cs::red(fstr("!!!Error!!! {}: ", __func__)) << fstr("idx={}超出索引范围！", idx) << endl;
			return {-1, -1};
		}
		return { dst_start, dst_end };
	}
}
