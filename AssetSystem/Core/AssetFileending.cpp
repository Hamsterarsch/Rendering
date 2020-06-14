#pragma once
#include "AssetFileending.hpp"


namespace assetSystem
{
	const char *GetAssetFileending()
	{
		static const char *fileending{ ".asset" };
		
		return fileending;
		
	}


	
	const wchar_t *GetAssetFileendingW()
	{
		static const wchar_t *fileending{ L".asset" };
		
		return fileending;
		
	}

	
}

