//base
#pragma once
#include<type_traits>

#include"sign.hpp"


namespace mineutils
{
	using std::decay;
	using std::is_same;
/*--------------------------------------------����----------------------------------------------*/
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

/*--------------------------------------------����----------------------------------------------*/
	template<class T1, class T2, class ...Ts>
	inline bool isSameType()
	{
		/*
		�����ж������ǲ�����ͬ���͡�
		-��������಻����ͬ����;
		-�Ƚ�����ʱ����const��&���Σ�����ͬ��������ĳ��Ȳ�ͬ��
			-int��const int�ж���ͬ����int[]��const int[]�жϲ�ͬ��
			-�����ǳ���ָ����ָ�볣���жϲ�ͬ��ָ����ָ�볣���ж���ͬ��
		*/
		constexpr int type_id = (sizeof...(Ts) > 0);
		auto type_tag = std::get<type_id>(Sign::BOOL_TAGS);
		return _isSameType<T1, T2, Ts...>(type_tag);
	}

	template<class T1, class T2, class ...Ts>
	inline bool isSameType(T1& arg1, T2& arg2, Ts& ...args)
	{
		/*
		�����ж���������ǲ�����ͬ���͡�
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
		�����ж�T�Ƿ����ں���Ķ������͡�
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