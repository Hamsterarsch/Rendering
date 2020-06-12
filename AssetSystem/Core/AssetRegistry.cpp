#include "AssetSystem/Core/AssetRegistry.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Shared/Hashing/CRC32.hpp"


namespace assetSystem::Core
{
	const char *AssetRegistry::assetExtension{ ".asset" };
	
	AssetRegistry::AssetRegistry(const char *projectAssetDirectory) : 
		programDirectory{ Filesystem::Conversions::MakeExeRelative("../../ProgramAssets/") },
		projectAssetDirectory{ projectAssetDirectory }
	{
		DiscoverAssets(programDirectory);
		DiscoverAssets(projectAssetDirectory);
		
	}

		void AssetRegistry::DiscoverAssets(const fs::path &rootFolder)
		{			
			for(auto &&entry : fs::recursive_directory_iterator{ rootFolder })
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


	
	fs::path AssetRegistry::GetAssetPath(const unsigned assetKey) const
	{
		return fileHandleMap.at(assetKey);
		
	}

	
}
