#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "AssetSystemExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "AssetSystemTypes.hpp"
#include "Asset.hpp"


namespace assetSystem
{
	struct LoadedAssetInfo
	{
		Asset *asset;
		AssetKey key;
	};
	
	class ASSET_SYSTEM_DLLSPEC AssetSystem
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AssetSystem)

		public: virtual Asset *GetAsset(AssetKey key) = 0;

		public: virtual LoadedAssetInfo GetAsset(const char *relativePath) = 0;

		public: virtual void RemoveReference(AssetKey key) = 0;

		public: virtual void AddReference(AssetKey key) = 0;
				
	};

	UniquePtr<AssetSystem> ASSET_SYSTEM_DLLSPEC MakeAssetSystem(const char *projectAssetFolder);
	
}
