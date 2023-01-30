#pragma once
#include<tuple>

namespace Sign
{
	class CaseTag0 {};
	class CaseTag1 {};

	inline std::tuple<CaseTag0, CaseTag1> _creat_BOOL_TAGS()
	{
		CaseTag0 tag0;
		CaseTag1 tag1;
		std::tuple<CaseTag0, CaseTag1> BOOL_TAGS(tag0, tag1);
		return BOOL_TAGS;
	}

	static std::tuple<CaseTag0, CaseTag1> BOOL_TAGS = _creat_BOOL_TAGS();


//·ÏÆú´úÂë
/*	class CaseTag0;
	class CaseTag1;


	static CaseTag0* tag0_ptr = nullptr;
	static CaseTag1* tag1_ptr = nullptr;

	class CaseTag0
	{
	private:
		CaseTag0() {}
		CaseTag0(CaseTag0& tag) {}

	public:
		static CaseTag0& getInstance()
		{
			if (tag0_ptr == nullptr)
			{
				CaseTag0 tag0;
				tag0_ptr = &tag0;
			}
			return *tag0_ptr;
		}
	};

	class CaseTag1
	{
	private:
		CaseTag1() {}
		CaseTag1(CaseTag1& tag) {}

	public:
		static CaseTag1& getInstance()
		{
			if (tag1_ptr == nullptr)
			{
				CaseTag1 tag0;
				tag1_ptr = new CaseTag1;
			}
			return *tag1_ptr;
		}
	};

	inline tuple<CaseTag0&, CaseTag1&> _creat_BOOL_TAGS()
	{
		CaseTag0& tag0 = CaseTag0::getInstance();
		CaseTag1& tag1 = CaseTag1::getInstance();
		tuple<CaseTag0&, CaseTag1&> BOOL_TAGS(tag0, tag1);
		return BOOL_TAGS;
	}

	static tuple<CaseTag0&, CaseTag1&> BOOL_TAGS = _creat_BOOL_TAGS();
*/
}