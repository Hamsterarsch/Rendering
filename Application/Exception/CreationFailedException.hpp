#pragma once
#include "Exception.hpp"


namespace Exception
{
	class CreationFailed : public Exception
	{
		public: CreationFailed(const char *description) :
			Exception(description)
		{}

		
	};
	
	
}
