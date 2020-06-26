#pragma once
#include "Archivable.hpp"


namespace assetSystem
{
	class ASSET_SYSTEM_DLLSPEC Asset : public Archivable
	{
		public: virtual void OnMakeAsset(const char *absoluteAssetFilePath) {}

		public: virtual void OnAssetLoaded(const char *absoluteAssetFilePath) {}
		
	};

	
}
