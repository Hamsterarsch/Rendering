#pragma once
#include "AssetSystemTypes.hpp"
#include "IO/Archive.hpp"
#include "Archivable.hpp"
#include <filesystem>
#include <unordered_map>


namespace assetSystem::core
{
	namespace fs = std::filesystem;
	
	class AssetRegistry : public Archivable
	{		
		private: fs::path projectAssetDirectory;
				 		
		private: std::unordered_map<AssetKey, fs::path> fileHandleMap;

		private: struct ReferenceBucket
		{
			unsigned strongReferences{ 0 };
			unsigned weakReferences{ 0 };
			
		};

		private: std::unordered_map<AssetKey, ReferenceBucket> references;
				 		

		
		public: AssetRegistry(const char *projectAssetDirectory);

			private: void DiscoverAssets(const fs::path &rootFolder);

		
		public: fs::path GetAssetDirectory() const { return projectAssetDirectory; }
		
		public: void RegisterAsset(const fs::path &projectRelativePath);

			private: static void ReplaceBackslashes(std::string &path);
		
		public: static AssetKey MakeAssetKey(const std::string &projectRelativePath);

		public: fs::path GetAbsoluteAssetPath(AssetKey assetKey) const;



		public: void AddReference(AssetKey key);

		public: void RemoveReference(AssetKey key);

		public: bool IsUnreferenced(AssetKey key);
		
		public: bool IsAssetKnown(AssetKey key) const;

		public: static AssetKey MakeAssetKey(const char *projectRelativePath);

		public: io::Archive &Serialize(io::Archive &archive) override{ return archive; }
	};
	
	
}


