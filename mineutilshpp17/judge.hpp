//base
#pragma once
#include<type_traits>

using std::decay;
using std::is_same;


namespace mineutils
{
	template<class T1, class T2, class ...Ts>
	constexpr bool isSameType()
	{
		/*
		用于判断类型是不是相同类型。
		-父类和子类不是相同类型;
		-比较类型时忽略const和&修饰，忽略同类型数组的长度不同；
			-int和const int判断相同，但int[]和const int[]判断不同；
			-本质是常量指针与指针常量判断不同，指针与指针常量判断相同。
		*/

		if constexpr (sizeof...(Ts) > 0)
			return is_same<typename decay<T1>::type, typename decay<T2>::type>::value and isSameType<T1, Ts...>();
		else return is_same<typename decay<T1>::type, typename decay<T2>::type>::value;
	}

	template<class T1, class T2, class ...Ts>
	constexpr bool isSameType(T1& arg1, T2& arg2, Ts& ...args)
	{
		/*
		用于判断输入参数是不是相同类型。
		*/
		return isSameType<T1, T2, Ts...>();
	}


	template<class T, class T1, class... Types>
	constexpr bool isInTypes()
	{
		/*
		用于判断T是否属于后面的多种类型中。
		*/
		if constexpr (sizeof...(Types) > 0)
			return isSameType<T, T1>() or isInTypes<T, Types...>();
		else return isSameType<T, T1>();
	}
}