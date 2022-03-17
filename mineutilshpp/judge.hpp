#pragma once
#include<type_traits>


using std::cout;
using std::decay;
using std::is_same;


template<class T1, class T2>
constexpr bool isSameType(bool use_decay = true)
{
	/*
	用于判断两个类型是不是相同类型。
	-父类和子类不是相同类型;
	-如果use_decay为true，则比较类型时忽略const和&修饰，忽略同类型数组的长度不同；
		--坑：int和const int判断相同，但int[]和const int[]判断不同；
	-如果use_decay为false， 两个类型需严格相同。
	*/
	if (use_decay)
		return is_same<typename decay<T1>::type, typename decay<T2>::type>::value;
	else
		return is_same<T1, T2>::value;
}

template<class T1, class T2>
constexpr bool isSameType(T1& arg1, T2& arg2, bool use_decay = true)
{
	/*
	用于判断两个输入参数是不是相同类型。
	*/
	return isSameType<T1, T2>(use_decay);
}


template<class T, class T1, class... Types>
constexpr bool isInTypes(bool use_decay = true)
{
	/*
	用于判断T是否属于后面的多种类型中。
	*/
	if constexpr (sizeof...(Types) > 0)
		return isSameType<T, T1>(use_decay) or isInTypes<T, Types...>(use_decay);
	else
		return isSameType<T, T1>(use_decay);
}