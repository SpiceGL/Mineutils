#pragma once
#include<iostream>

namespace sign
{
	class All
	{
	private:
		All() {}
		All(const All& a) {}
		static All* self;

		class SelfGarbo
		{
		public:
			~SelfGarbo()
			{
				if (self)
				{
					delete self;
				}
			}
		};
		static SelfGarbo self_garbo;

	public:
		static All& getInstance()
		{
			if (self == nullptr)
				self = new All;
			return *self;
		}
	};

	All* All::self = nullptr;
	All& ALL = All::getInstance();
}