#include "AssetTypes/CacheAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"
#include "Shared/Exception/Exception.hpp"



namespace App::Assets
{
	CacheAsset::CacheAsset(const int sizeInBytes)
	{
		data.Resize(sizeInBytes);
		
	}


	
	assetSystem::io::Archive &CacheAsset::Serialize(assetSystem::io::Archive &archive)
	{		
		if(data.GetSizeInBytes() > static_cast<unsigned>(std::numeric_limits<int>::max()))
		{
			throw Exception::Exception{"CacheAsset: cache size is to big" };
			
		}
		int size{ static_cast<int>(data.GetSizeInBytes()) };
		
		archive.Serialize("cacheSize", size);

		
		if(data.GetSizeInBytes() < static_cast<unsigned>(size))//size can never be negative
		{
			data.Resize(static_cast<unsigned>(size));
		}
		
		return archive.Serialize("cache", data.GetData(), static_cast<unsigned>(size), sizeof(unsigned char) );
						
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
