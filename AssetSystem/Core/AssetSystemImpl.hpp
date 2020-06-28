#pragma once
#include "AssetSystem.hpp"
#include "AssetRegistry.hpp"
#include "AssetMemory.hpp"


namespace assetSystem::core
{
	class AssetSystemImpl final : public AssetSystem
	{
		private: AssetRegistry registry;

		private: AssetMemory memory;


		
		public: AssetSystemImpl(AssetRegistry &&registry, AssetMemory &&memory);
		

		public:	void RegisterAssetClass(const char* classFileExtension, UniquePtr<AssetConstructionOperations> &&constructOperations) override;
		
		public:	AssetPtr MakeAsset(const char *path, Asset &&assetData) override;

			private: std::string EnsureProjectRelativePath(const char *path) const;
		
			private: static std::string GetAssetClassExtension(const char *projectRelativePath);
		
		public: AssetPtr GetAsset(const char *projectRelativePath) override;

		public:	bool DoesAssetExist(const char *projectRelativePath) const override;
		
		public:	void SerializeAsset(AssetPtr &assetPtr) override;

		public: void ReloadAsset(AssetPtr &assetPtr) override;
		
						
		private: LoadedAssetInfo GetAssetInternal(const char *projectRelativePath) override;
				
		private: void RemoveReference(AssetKey key) override;
		
		private: void AddReference(AssetKey key) override;


	};
	
	
}
