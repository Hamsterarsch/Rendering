#include "Exception/Exception.hpp"

namespace Exception
{
	Exception::Exception(const char *description) :
		std::exception(description)
	{}

	
}
