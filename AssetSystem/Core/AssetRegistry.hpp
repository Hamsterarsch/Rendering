#pragma once
#include "AssetSystemTypes.hpp"
#include "AssetSystem/IO/Archive.hpp"
#include <filesystem>
#include <unordered_map>


namespace assetSystem::Core
{
	namespace fs = std::filesystem;
	
	class AssetRegistry : public IO::Archivable
	{
		private: static const char *assetExtension;

		private: fs::path programDirectory;

		private: fs::path projectAssetDirectory;
				 		
		private: std::unordered_map<AssetKey, fs::path> fileHandleMap;

		private: struct ReferenceBucket
		{
			unsigned strongReferences;
			unsigned weakReferences;
			
		};

		private: std::unordered_map<AssetKey, ReferenceBucket> references;
		

		
		public: AssetRegistry(const char *projectAssetDirectory);

			private: void DiscoverAssets(const fs::path &rootFolder);

		public: void RegisterAsset(const fs::path &path);

		public: fs::path GetAssetPath(AssetKey assetKey) const;
						 		

		public: IO::Archive &Serialize(IO::Archive &archive) override{ return archive; }
	};
	
	
}

