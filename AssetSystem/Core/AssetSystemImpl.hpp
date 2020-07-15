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

			private: std::string EnsureCorrectPathFormat(const char *path) const;

				private: static std::string ReplaceBackslashes(std::string &&path);
		
			private: Asset &ConstructAsset(AssetKey key, std::string &&projectRelativePath, Asset &&assetData);
				
				private: static std::string GetAssetClassExtension(const char *projectRelativePath);

			private: void WriteAssetToFile(AssetKey key, Asset &asset) const;
		
		public: AssetPtr GetAsset(const char *projectRelativePath) override;

		public:	bool DoesAssetExist(const char *projectRelativePath) const override;
		
		public:	void SerializeAsset(AssetPtr &assetPtr) override;

		public: void ReloadAsset(AssetPtr &assetPtr) override;
		
		public: bool IsSameRootAssetPath(const char *asThisPath) const override;
		
		public: std::string GetAbsoluteRootAssetPath() const override;
		
						
		private: LoadedAssetInfo GetAssetInternal(const char *projectRelativePath) override;

			private: void WriteFileToAsset(AssetKey key, Asset &asset) const;
		
		private: void RemoveReference(AssetKey key) override;
		
		private: void AddReference(AssetKey key) override;


	};

	
}
