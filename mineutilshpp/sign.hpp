#pragma once
#include<iostream>
#include<tuple>

using std::tuple;

namespace Sign
{
	class CaseTag0
	{
	private:
		CaseTag0() {}
		CaseTag0(CaseTag0& tag) {}
		static CaseTag0* self;

	public:
		static CaseTag0& getInstance()
		{
			if (self == nullptr)
			{
				CaseTag0 tag0;
				self == &tag0;
			}
			return *self;
		}
	};

	class CaseTag1
	{
	private:
		CaseTag1() {}
		CaseTag1(CaseTag1& tag) {}
		static CaseTag1* self;

	public:
		static CaseTag1& getInstance()
		{
			if (self == nullptr)
			{
				CaseTag1 tag0;
				self == &tag0;
			}
			return *self;
		}
	};

	CaseTag0* CaseTag0::self = nullptr;
	CaseTag1* CaseTag1::self = nullptr;

	tuple<CaseTag0&, CaseTag1&> _creat_BOOL_TAGS()
	{
		CaseTag0& tag0 = CaseTag0::getInstance();
		CaseTag1& tag1 = CaseTag1::getInstance();
		tuple<CaseTag0&, CaseTag1&> BOOL_TAGS(tag0, tag1);
		return BOOL_TAGS;
	}

	tuple<CaseTag0&, CaseTag1&> BOOL_TAGS = _creat_BOOL_TAGS();
}