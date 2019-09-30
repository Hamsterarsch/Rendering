#pragma once

#ifdef RHA_DLLCOMPILE
	#define RHA_DLLSPEC __declspec(dllexport)
#elif
	#define RHA_DLLSPEC __declspec(dllimport)
#endif

