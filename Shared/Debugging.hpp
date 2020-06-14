#pragma once


namespace Exception
{
	inline void DebugBreak()
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		
	}

	
}