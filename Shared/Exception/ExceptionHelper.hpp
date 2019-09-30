#pragma once

#define DEFAULT_EXCEPTION_TYPE_DEFINITION(Name)			\
														\
#include "Exception.hpp"								\
														\
namespace Exception										\
{														\
	class Name : public Exception						\
	{													\
		public: Name(const char *description) :			\
		Exception(description)							\
		{}												\
														\
	};													\
}

