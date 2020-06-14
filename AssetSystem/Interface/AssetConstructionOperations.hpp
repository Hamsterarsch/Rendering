#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace assetSystem
{
	class Asset;
	
	class AssetConstructionOperations
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AssetConstructionOperations)

		public: virtual Asset &ConstructAsset(char *memory) const = 0;

		public:	virtual Asset &ConstructAsset(char *memory, Asset &&assetData) const = 0;
		
		public: virtual size_t GetAssetSizeInBytes() const = 0;
		
		
	};

	
}
