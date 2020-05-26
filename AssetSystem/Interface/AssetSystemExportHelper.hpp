#pragma once

#ifdef ASSET_SYSTEM_DLLCOMPILE
	#define ASSET_SYSTEM_DLLSPEC __declspec(dllexport)
#else
	#define ASSET_SYSTEM_DLLSPEC __declspec(dllimport)
#endif


