#include "AssetSystem/Core/AssetRegistry.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Shared/Hashing/CRC32.hpp"


namespace AssetSystem
{
	const char *AssetRegistry::assetExtension{ ".asset" };
	
	AssetRegistry::AssetRegistry() :
		configDir{ Filesystem::Conversions::MakeExeRelative("../../Config/") },
		config{ configDir }
	{
		DiscoverAssets();
		
	}

		void AssetRegistry::DiscoverAssets()
		{
			const auto assetDirectory{ config.GetAssetDirectory() };
			for(auto &&entry : fs::recursive_directory_iterator{ assetDirectory })
			{
				if(entry.is_regular_file() && entry.path().extension() == assetExtension)
				{
					RegisterAsset(entry.path());
				}
			}
		
		}

			void AssetRegistry::RegisterAsset(const fs::path &path)
			{
				const auto asString{ path.string() };
				const auto hash{ Hashing::CRC32Mpeg2::Generate(asString.data(), asString.size()) };

				fileHandleMap[hash] = path;
		
			}

	
}
