#pragma once
#include "AssetSystemTypes.hpp"
#include <unordered_map>
#include "Shared/PtrTypes.hpp"
#include "Asset.hpp"


namespace assetSystem
{
	class AssetMemory
	{
		private: std::unordered_map<AssetKey, UniquePtr<Asset>> assets;


		
		void StoreAsset(AssetKey assetKey, UniquePtr<Asset> &&asset) { assets[assetKey] = std::move(asset); }

		Asset &GetAsset(AssetKey assetKey) { return *assets.at(assetKey); }

		void FreeAsset(AssetKey assetKey) { assets.erase(assetKey); }
		
	};
	
	
}
