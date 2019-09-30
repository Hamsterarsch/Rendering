#pragma once
#include "Exception.hpp"

#define DEFAULT_EXCEPTION_TYPE_DEFINITION(Name)			\
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

