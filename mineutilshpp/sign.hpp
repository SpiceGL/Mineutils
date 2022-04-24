#pragma once
#include<iostream>
#include<tuple>

using std::tuple;

namespace Sign
{
	struct CaseTag0	{};
	struct CaseTag1 {};

	CaseTag0 tag0;
	CaseTag1 tag1;
	tuple<CaseTag0, CaseTag1> Diode_TAGS(tag0, tag1);
}