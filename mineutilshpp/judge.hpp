//base
#pragma once
#include<type_traits>

#include"sign.hpp"


namespace mineutils
{
/*--------------------------------------------声明----------------------------------------------*/
	template<class T1, class T2, class ...Ts>
	constexpr bool isSameType();

	template<class T1, class T2, class ...Ts>
	constexpr bool isSameType(T1& arg1, T2& arg2, Ts& ...args);

	template<class T1, class T2, class ...Ts>
	constexpr bool _isSameType(Sign::CaseTag1 tag);

	template<class T1, class T2>
	constexpr bool _isSameType(Sign::CaseTag0 tag);

	template<class T, class T1, class... Types>
	constexpr bool isInTypes();

	template<class T, class T1, class... Types>
	constexpr bool _isInTypes(Sign::CaseTag1 tag);

	template<class T, class T1>
	constexpr bool _isInTypes(Sign::CaseTag0 tag);

/*--------------------------------------------定义----------------------------------------------*/

	
	/*
		用于判断类型是不是相同类型。
		-父类和子类不是相同类型;
		-比较类型时忽略const和&修饰，忽略同类型数组的长度不同；
			-int和const int判断相同，但int[]和const int[]判断不同；
			-涉及指针的时候忽略不了const。
	*/
	template<class T1, class T2, class ...Ts>
	inline constexpr bool isSameType()
	{
		constexpr int type_id = (sizeof...(Ts) > 0);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		return _isSameType<T1, T2, Ts...>(type_tag);
	}

	//用于判断输入参数是不是相同类型。
	template<class T1, class T2, class ...Ts>
	inline constexpr bool isSameType(T1& arg1, T2& arg2, Ts& ...args)
	{
		return isSameType<T1, T2, Ts...>();
	}

	template<class T1, class T2, class ...Ts>
	inline constexpr bool _isSameType(Sign::CaseTag1 tag)
	{
		return std::is_same<typename std::decay<T1>::type, typename std::decay<T2>::type>::value and isSameType<T1, Ts...>();
	}

	template<class T1, class T2>
	inline constexpr bool _isSameType(Sign::CaseTag0 tag)
	{
		return std::is_same<typename std::decay<T1>::type, typename std::decay<T2>::type>::value;
	}

	//用于判断T是否属于后面的多种类型。
	template<class T, class T1, class... Types>
	inline constexpr bool isInTypes()
	{
		constexpr int type_id = (sizeof...(Types) > 0);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		return _isInTypes<T, T1, Types...>(type_tag);
	}

	template<class T, class T1, class... Types>
	inline constexpr bool _isInTypes(Sign::CaseTag1 tag)
	{
		return isSameType<T, T1>() or isInTypes<T, Types...>();
	}

	template<class T, class T1>
	inline constexpr bool _isInTypes(Sign::CaseTag0 tag)
	{
		return isSameType<T, T1>();
	}
}