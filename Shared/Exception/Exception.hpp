#pragma once
#include <exception>


namespace Exception
{	
	class Exception : public std::exception
	{
		public: Exception(const char *description);

		
	};

	inline void ThrowIfDebug(const Exception &exception)
	{
		#if _DEBUG
			throw exception;
		#endif
	}

	inline void ThrowIfDebug(const bool shouldThrow, const Exception &exception)
	{
		#if _DEBUG
		if(shouldThrow)
		{
			throw exception;			
		}
		#endif
	}

}
