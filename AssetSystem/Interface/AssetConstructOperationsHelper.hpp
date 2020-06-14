#pragma once
#include "AssetConstructionOperations.hpp"
#include <type_traits>
#include "Asset.hpp"


namespace assetSystem
{
	template<class t_asset, bool = std::is_base_of_v<Asset, t_asset>>
	class AssetConstructOperationsHelper final : public AssetConstructionOperations
	{		
		public:	Asset &ConstructAsset(char *memory) const override
		{
			return *(new(memory) t_asset());
			
		}
		
		public:	Asset &ConstructAsset(char *memory, Asset &&assetData) const override
		{
			return *(new(memory) t_asset(std::move(reinterpret_cast<t_asset &&>(assetData))));
			
		}

		size_t GetAssetSizeInBytes() const override
		{
			return sizeof t_asset;
		}
		
	};

	template<class t_asset>
	class AssetConstructOperationsHelper<t_asset, false>
	{
		static_assert("AssetConstructOperations argument type must be derived of assetSystem::Asset");
	};

	
}
