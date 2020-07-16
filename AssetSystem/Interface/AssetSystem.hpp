#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "AssetSystemExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "AssetSystemTypes.hpp"
#include "AssetPtr.hpp"
#include "Asset.hpp"
#include <string>


namespace assetSystem
{
	struct LoadedAssetInfo
	{
		Asset *asset;
		AssetKey key;
	};
	
	class AssetConstructionOperations;
	
	class ASSET_SYSTEM_DLLSPEC AssetSystem
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AssetSystem)
				

		public: virtual void RegisterAssetClass(const char *classFileExtension, UniquePtr<AssetConstructionOperations> &&constructOperations) = 0;


				
		public: virtual AssetPtr MakeAsset(const char *projectRelativePath, Asset &&assetData) = 0;

		public: virtual AssetPtr GetAsset(const char *projectRelativePath) = 0;

		public: virtual bool DoesAssetExist(const char *projectRelativePath) const = 0;
		
		public: virtual void SerializeAsset(AssetPtr &assetPtr) = 0;

		public: virtual void ReloadAsset(AssetPtr &assetPtr) = 0;

		public: virtual bool IsSameRootAssetPath(const char *asThisPath) const = 0;

		public: virtual std::string GetAbsoluteRootAssetPath() const = 0;


		public: virtual void DeleteAsset(const char *projectRelativePathToDelete) = 0;
		
		public: virtual void DeleteAsset(const char *projectRelativePathToDelete, const char *projectRelativePathToReplacementAsset) = 0;

		public: virtual void MoveAsset(const char *projectRelativeSourcePath, const char *projectRelativeTargetPath);
						
		
		friend AssetPtr;

			private: virtual LoadedAssetInfo GetAssetInternal(const char *relativePath) = 0;

			private: virtual void RemoveReference(AssetKey key) = 0;

			private: virtual void AddReference(AssetKey key) = 0;

			private: virtual bool AssetWasInvalidated(AssetKey key) const = 0;
					 					 		
			private: virtual LoadedAssetInfo GetUpdatedAssetData(AssetKey oldKey) = 0;
					 		
	};

	UniquePtr<AssetSystem> ASSET_SYSTEM_DLLSPEC MakeAssetSystem(const char *projectAssetFolder);

	
	
}
