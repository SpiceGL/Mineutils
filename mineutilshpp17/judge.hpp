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
		�����ж������ǲ�����ͬ���͡�
		-��������಻����ͬ����;
		-�Ƚ�����ʱ����const��&���Σ�����ͬ��������ĳ��Ȳ�ͬ��
			-int��const int�ж���ͬ����int[]��const int[]�жϲ�ͬ��
			-�����ǳ���ָ����ָ�볣���жϲ�ͬ��ָ����ָ�볣���ж���ͬ��
		*/

		if constexpr (sizeof...(Ts) > 0)
			return is_same<typename decay<T1>::type, typename decay<T2>::type>::value and isSameType<T1, Ts...>();
		else return is_same<typename decay<T1>::type, typename decay<T2>::type>::value;
	}

	template<class T1, class T2, class ...Ts>
	constexpr bool isSameType(T1& arg1, T2& arg2, Ts& ...args)
	{
		/*
		�����ж���������ǲ�����ͬ���͡�
		*/
		return isSameType<T1, T2, Ts...>();
	}


	template<class T, class T1, class... Types>
	constexpr bool isInTypes()
	{
		/*
		�����ж�T�Ƿ����ں���Ķ��������С�
		*/
		if constexpr (sizeof...(Types) > 0)
			return isSameType<T, T1>() or isInTypes<T, Types...>();
		else return isSameType<T, T1>();
	}
}