#pragma once
#include <filesystem>


namespace assetSystem::Assets
{
	class AssetSystemConfigAsset
	{
		private: static inline constexpr const char *assetDirectoryPropertyName{ "assetDirectory" };
		
		private: std::string assetDirectory;


		
		public: AssetSystemConfigAsset(const std::filesystem::path &configDirectory);

		public: std::filesystem::path GetAssetDirectory() const;
		
	};
	
	
}
