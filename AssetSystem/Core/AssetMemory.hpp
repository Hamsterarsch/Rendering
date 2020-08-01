#pragma once
#include "AssetSystemTypes.hpp"
#include <unordered_map>
#include "Shared/PtrTypes.hpp"
#include "Asset.hpp"

#include "AssetConstructionOperations.hpp"


namespace assetSystem::core
{
	class AssetMemory
	{
		struct AssetInfo
		{
			UniquePtr<char[]> memory;
			AssetConstructionOperations *cOps;
		};
		
		private: std::unordered_map<AssetKey, AssetInfo> assetInfos;

		private: std::unordered_map<std::string, UniquePtr<AssetConstructionOperations>> assetConstructOperations;
		

		public: void RegisterAssetClass(const char *classFileExtension, UniquePtr<AssetConstructionOperations> &&constructOperations);

		
		public: Asset &MakeAsset(Asset &&assetData, AssetKey key, const char *classFileExtension);
		
			private: char *AllocateAssetMemory(AssetKey key, AssetConstructionOperations &cOps);

		public: Asset &MakeAsset(AssetKey key, const char *classFileExtension);
		
		public: Asset &GetAsset(AssetKey key);

		public: void FreeAsset(AssetKey key);

		public: void RenameAsset(AssetKey key, AssetKey newKey);
		
		public: bool IsAssetLoaded(AssetKey key) const;
		
	};
	
	
}
