#include "AssetSystem/Core/AssetRegistry.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Shared/Hashing/CRC32.hpp"


namespace assetSystem::core
{
	const char *AssetRegistry::assetExtension{ ".asset" };
	
	AssetRegistry::AssetRegistry(const char *projectAssetDirectory)		
	{
		std::string asString{ projectAssetDirectory };
		if(asString.back() != '/' || asString.back() != '\\')
		{
			asString.append("/");
		}		
		this->projectAssetDirectory = asString;
		
		DiscoverAssets(projectAssetDirectory);
		
	}

		void AssetRegistry::DiscoverAssets(const fs::path &rootFolder)
		{			
			for(auto &&entry : fs::recursive_directory_iterator{ rootFolder })
			{
				if(entry.is_regular_file() && entry.path().extension() == assetExtension)
				{				
					RegisterAsset(relative(entry, rootFolder));
				}
			}
		
		}

			void AssetRegistry::RegisterAsset(const fs::path &projectRelativePath)
			{
				auto asString{ projectRelativePath.string() };
				const auto extensionPos{ asString.find(assetExtension) };
				asString.erase(extensionPos, asString.size() - extensionPos);
		
				fileHandleMap[MakeAssetKey(asString)] = asString;
		
			}

				AssetKey AssetRegistry::MakeAssetKey(const std::string &projectRelativePath)
				{
					return Hashing::CRC32Mpeg2::Generate(projectRelativePath.data(), projectRelativePath.size());
		
				}

				
				
	fs::path AssetRegistry::GetAbsoluteAssetPath(const unsigned assetKey) const
	{
		return { projectAssetDirectory.string() + fileHandleMap.at(assetKey).string() + assetExtension };
		
	}


	
	void AssetRegistry::AddReference(const AssetKey key)
	{
		++references[key].strongReferences;
		
	}


	
	void AssetRegistry::RemoveReference(const AssetKey key)
	{
		if(--references[key].strongReferences == 0)
		{
			references.erase(key);	
		}
		
	}

	
	
	bool AssetRegistry::IsUnreferenced(AssetKey key)
	{
		return references.find(key) == references.end();
		
	}



	bool AssetRegistry::IsAssetKnown(const AssetKey key) const
	{			
		return fileHandleMap.find(key) != fileHandleMap.end();
		
	}

		AssetKey AssetRegistry::MakeAssetKey(const char *projectRelativePath)
		{
			return Hashing::CRC32Mpeg2::Generate(projectRelativePath, std::strlen(projectRelativePath));
		
		}


	
}
