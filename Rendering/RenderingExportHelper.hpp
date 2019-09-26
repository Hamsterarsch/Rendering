#pragma once

#ifdef RENDERING_DLLCOMPILE
	#define RENDERING_DLLSPEC __declspec(dllexport)
#elif
	#define RENDERING_DLLSPEC __declspec(dllimport)
#endif

