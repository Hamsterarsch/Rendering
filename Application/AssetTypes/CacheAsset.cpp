#include "AssetTypes/CacheAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Assets
{
	CacheAsset::CacheAsset(const int sizeInBytes) : dataSize{ sizeInBytes }, data{ MakeUnique<unsigned char[]>(sizeInBytes) }
	{
	}


	
	assetSystem::io::Archive &CacheAsset::Serialize(assetSystem::io::Archive &archive)
	{
		archive.Serialize("cacheSize", dataSize);

		if(!data)
		{
			data = MakeUnique<unsigned char[]>(dataSize);
		}
		
		return archive.Serialize("cache", data.get(), dataSize, sizeof(unsigned char) );
						
	}


	
	const char *CacheAsset::GetAssetClassExtension()
	{
		const char *extension{ "cache" };

		return extension;
		
	}


	
	const wchar_t *CacheAsset::GetAssetClassExtensionW()
	{
		const wchar_t *extension{ L"cache" };

		return extension;
		
	}

	
}
