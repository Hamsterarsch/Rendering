#pragma once
#include "AssetSystem.hpp"
#include "AssetRegistry.hpp"
#include "AssetMemory.hpp"
#include "InMemoryReferences.hpp"
#include "SerializedReferences.hpp"


namespace assetSystem::core
{
	class AssetSystemImpl final : public AssetSystem
	{
		private: AssetRegistry registry;

		private: InMemoryReferences ptrReferences;

		private: SerializedReferences serializedReferences;

		private: AssetMemory memory;


		
		public: AssetSystemImpl(AssetRegistry &&registry, AssetMemory &&memory);

			private: fs::path GetReferenceDataPath() const;
		
			private: void SaveReferenceData(bool forceRewrite = false);
		
			private: void LoadReferenceData();

		
		public:	void RegisterAssetClass(const char* classFileExtension, UniquePtr<AssetConstructionOperations> &&constructOperations) override;
		
		public:	AssetPtr MakeAsset(const char *assetPath, Asset &&assetData) override;

			private: std::string GetSafeAssetPathFromUnsafePath(const char *path) const;

			private: Asset &ConstructAsset(AssetKey key, std::string &&projectRelativePath, Asset &&assetData);
				
				private: static std::string GetAssetClassExtension(const char *projectRelativePath);

			private: void WriteAssetToFile(AssetKey key, Asset &asset);
		
		
		public: AssetPtr GetAsset(const char *assetPath) override;

		public:	bool DoesAssetExist(const char *assetPath) const override;

			private: AssetKey GetAssetKeyFromUnsafePath(const char *path) const;
		
		public:	void SerializeAsset(AssetPtr &assetPtr) override;

		public: void ReloadAsset(AssetPtr &assetPtr) override;
		
		public: bool IsSameRootAssetPath(const char *asThisPath) const override;
		
		public: std::string GetAbsoluteRootAssetPath() const override;
		
						
		public: bool TryToDeleteAsset(const char *assetPath) override;

			private: bool AssetCanBeDeletedWithoutReplacement(AssetKey key) const;
		
		public: void DeleteAsset(const char *pathToAssetToDelete, const char *pathToReplacementAsset) override;
		
			private: void ApplyAssetRedirectsToAssetFilesReferencing(AssetKey source);

				private: AssetPtr GetAssetFromPotentiallyRedirectedKey(AssetKey key);
		
		public: void MoveAsset(const char *sourceAssetPath, const char *targetAssetPath) override;

			private: void RedirectAsset(AssetKey source, AssetKey target);
					 		
		
		private: bool AssetWasInvalidated(AssetKey key) const override;
		
		private: LoadedAssetInfo GetUpdatedAssetData(AssetKey oldKey) override;
		
		private: LoadedAssetInfo GetAssetInternal(const char *projectRelativePath) override;

			private: void WriteFileToAsset(AssetKey key, Asset &asset);
		
		private: void RemoveReference(AssetKey key) override;
		
		private: void AddReference(AssetKey key) override;



	};

	
}
