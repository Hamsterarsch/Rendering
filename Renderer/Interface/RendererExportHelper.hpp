#pragma once


#ifdef RENDERER_DLLCOMPILE
	#define RENDERER_DLLSPEC __declspec(dllexport)
#else
	#define RENDERER_DLLSPEC __declspec(dllimport)
#endif
