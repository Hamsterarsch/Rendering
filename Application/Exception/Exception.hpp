#pragma once
#include <exception>


namespace Exception
{
	class Exception : public std::exception
	{
		public: Exception(const char *description) :
			std::exception(description)
		{}

		
	};


}
