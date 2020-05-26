#pragma once
#include "AssetSystem/IO/Archive.hpp"
#include "AssetSystem/Core/Assets/AssetSystemConfigAsset.hpp"
#include <filesystem>
#include <unordered_map>


namespace AssetSystem
{
	namespace fs = std::filesystem;
	
	class AssetRegistry : public IO::Archivable
	{
		private: static const char *assetExtension;

		private: fs::path configDir;

		private: Assets::AssetSystemConfigAsset config;

		private: std::unordered_map<unsigned, fs::path> fileHandleMap;

		private: struct ReferenceBucket
		{
			unsigned strongReferences;
			unsigned weakReferences;
			
		};

		private: std::unordered_map<unsigned, ReferenceBucket> references;
		
		
		public: AssetRegistry();

			private: void DiscoverAssets();

				private: void RegisterAsset(const fs::path &path);


		IO::Archive& Serialize(IO::Archive& archive) override{ return archive; }
	};
	
	
}

