#pragma once
#include<limits.h>
#include<stdlib.h>
#include"str.hpp"
#include"exception.hpp"


namespace mineutils
{
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
			std::cout << makeMessageW(__func__, fstr("idx={}超出索引范围！", idx)) << std::endl;
			return -1;
		}
		return normal_idx;
	}

	inline std::pair<int, int> normRange(std::pair<int, int> range, int len)
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

	inline std::pair<int, int> normRange(int idx, int len)
	{
		using cs = ColorStr;
		int dst_start, dst_end;
		if (idx >= 0 and idx < len)
			dst_start = idx, dst_end = idx + 1;
		else if (idx < 0 and idx >= -len)
			dst_start = idx + len, dst_end = idx + len + 1;
		else
		{
			std::cout << makeMessageW(__func__, fstr("idx={}超出索引范围！", idx)) << std::endl;
			return {-1, -1};
		}
		return { dst_start, dst_end };
	}
}
