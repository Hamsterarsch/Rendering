#pragma once


inline void DebugBreak()
{
#ifdef _DEBUG
	__debugbreak();
#endif
	
}
