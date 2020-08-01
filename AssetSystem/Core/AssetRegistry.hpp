#pragma once
#include "AssetSystemTypes.hpp"
#include <filesystem>
#include <unordered_map>


namespace assetSystem::core
{
	namespace fs = std::filesystem;
	
	class AssetRegistry
	{		
		private: fs::path projectAssetDirectory;
				 		
		private: std::unordered_map<AssetKey, fs::path> fileHandleMap;

		private: std::unordered_map<AssetKey, AssetKey> assetRedirects;

		
				 		
		public: AssetRegistry(const char *projectAssetDirectory);

			private: void CleanAndSaveRootDirectory(const char *path);
		
			private: void DiscoverAssets(const fs::path &rootFolder);

		
		public: fs::path GetAssetDirectory() const { return absolute(projectAssetDirectory); }
		
		public: void RegisterAsset(std::string &&projectRelativePath);

		public: bool IsAssetRegistered(AssetKey key) const;
		
		public: fs::path GetAbsoluteAssetPath(AssetKey assetKey) const;


		public: void AddAssetRedirect(AssetKey from, AssetKey to);

		public: bool HasAssetRedirect(AssetKey key) const;

		public: AssetKey GetAssetRedirect(AssetKey key) const;


		public: void UnregisterAsset(AssetKey key);
						
	};

	
}


