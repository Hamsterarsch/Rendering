#include "AssetSystem/Core/AssetRegistry.hpp"
#include "AssetHashingUtils.hpp"
#include "AssetFileending.hpp"
#include "Shared/Exception/Exception.hpp"


namespace assetSystem::core
{	
	AssetRegistry::AssetRegistry(const char *projectAssetDirectory)		
	{		
		CleanAndSaveRootDirectory(projectAssetDirectory);
		DiscoverAssets(projectAssetDirectory);
		
	}

		void AssetRegistry::CleanAndSaveRootDirectory(const char *path)
		{
			std::string asString{ path };
			if(asString.back() != '/' && asString.back() != '\\')
			{
				asString.append("/");
			}		
			this->projectAssetDirectory = asString;
			
		}
			
		void AssetRegistry::DiscoverAssets(const fs::path &rootFolder)
		{			
			for(auto &&entry : fs::recursive_directory_iterator{ rootFolder })
			{
				if(entry.is_regular_file() && entry.path().extension() == GetAssetFileending())
				{				
					RegisterAsset(relative(entry, rootFolder).string());
				}
			}
		
		}

			void AssetRegistry::RegisterAsset(std::string &&projectRelativePath)
			{
				auto cleanPath{ EnsureCorrectPathFormat(projectRelativePath.c_str()) };
				const auto key{ MakeAssetKey(cleanPath) };
				fileHandleMap.insert( {key, std::move(cleanPath)} );
		
			}


				
	bool AssetRegistry::IsAssetRegistered(const AssetKey key) const
	{			
		return fileHandleMap.find(key) != fileHandleMap.end();
		
	}

	
				
	fs::path AssetRegistry::GetAbsoluteAssetPath(const unsigned assetKey) const
	{
		return { projectAssetDirectory.string() + fileHandleMap.at(assetKey).string() + GetAssetFileending() };
		
	}


	
	void AssetRegistry::AddAssetRedirect(const AssetKey from, const AssetKey to)
	{
		Exception::ThrowIfDebug(HasAssetRedirect(from), {"AssetRegistry: Tried to overwrite an existing redirect"});

		assetRedirects[from] = to;
		
	}
	
		bool AssetRegistry::HasAssetRedirect(const AssetKey key) const
		{
			return assetRedirects.find(key) != assetRedirects.end();
			
		}


	
	AssetKey AssetRegistry::GetAssetRedirect(const AssetKey key) const
	{
		return assetRedirects.at(key);
		
	}



	void AssetRegistry::UnregisterAsset(const AssetKey key)
	{
		assetRedirects.erase(key);
		fileHandleMap.erase(key);
		
	}

	   	
}
