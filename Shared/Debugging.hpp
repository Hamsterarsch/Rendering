#pragma once


namespace Exception
{
	inline void DebugBreak()
	{
#ifdef _DEBUG
#ifndef DISABLE_BREAKS
		__debugbreak();
#endif
#endif
		
	}

	
}