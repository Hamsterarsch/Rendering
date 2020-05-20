#pragma once
#include <exception>


namespace Exception
{	
	class Exception : public std::exception
	{
		public: Exception(const char *description);

		
	};

	inline void ThrowIfDebug(Exception exception)
	{
		#if _DEBUG
			throw exception;
		#endif
	}
	

}
