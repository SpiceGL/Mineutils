//base
#pragma once
#include<type_traits>

#include"sign.hpp"


namespace mineutils
{
	using std::decay;
	using std::is_same;
/*--------------------------------------------声明----------------------------------------------*/
	template<class T1, class T2, class ...Ts>
	bool isSameType();

	template<class T1, class T2, class ...Ts>
	bool isSameType(T1& arg1, T2& arg2, Ts& ...args);

	template<class T1, class T2, class ...Ts>
	bool _isSameType(Sign::CaseTag1 tag);

	template<class T1, class T2>
	bool _isSameType(Sign::CaseTag0 tag);

	template<class T, class T1, class... Types>
	bool isInTypes();

	template<class T, class T1, class... Types>
	bool _isInTypes(Sign::CaseTag1 tag);

	template<class T, class T1>
	bool _isInTypes(Sign::CaseTag0 tag);

/*--------------------------------------------定义----------------------------------------------*/
	template<class T1, class T2, class ...Ts>
	inline bool isSameType()
	{
		/*
		用于判断类型是不是相同类型。
		-父类和子类不是相同类型;
		-比较类型时忽略const和&修饰，忽略同类型数组的长度不同；
			-int和const int判断相同，但int[]和const int[]判断不同；
			-本质是常量指针与指针常量判断不同，指针与指针常量判断相同。
		*/
		constexpr int type_id = (sizeof...(Ts) > 0);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		return _isSameType<T1, T2, Ts...>(type_tag);
	}

	template<class T1, class T2, class ...Ts>
	inline bool isSameType(T1& arg1, T2& arg2, Ts& ...args)
	{
		/*
		用于判断输入参数是不是相同类型。
		*/
		return isSameType<T1, T2, Ts...>();
	}

	template<class T1, class T2, class ...Ts>
	inline bool _isSameType(Sign::CaseTag1 tag)
	{
		return is_same<typename decay<T1>::type, typename decay<T2>::type>::value and isSameType<T1, Ts...>();
	}

	template<class T1, class T2>
	inline bool _isSameType(Sign::CaseTag0 tag)
	{
		return is_same<typename decay<T1>::type, typename decay<T2>::type>::value;
	}


	template<class T, class T1, class... Types>
	inline bool isInTypes()
	{
		/*
		用于判断T是否属于后面的多种类型。
		*/
		constexpr int type_id = (sizeof...(Types) > 0);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		return _isInTypes<T, T1, Types...>(type_tag);
	}

	template<class T, class T1, class... Types>
	inline bool _isInTypes(Sign::CaseTag1 tag)
	{
		return isSameType<T, T1>() or isInTypes<T, Types...>();
	}

	template<class T, class T1>
	inline bool _isInTypes(Sign::CaseTag0 tag)
	{
		return isSameType<T, T1>();
	}
}