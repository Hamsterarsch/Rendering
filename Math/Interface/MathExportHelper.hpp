#pragma once


#ifdef MATH_DLLCOMPILE
	#define MATH_DLLSPEC __declspec(dllexport)
#else
	#define MATH_DLLSPEC __declspec(dllimport)
#endif

