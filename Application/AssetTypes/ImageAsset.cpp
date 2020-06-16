#include "AssetTypes/ImageAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Assets
{
	ImageAsset::ImageAsset() : sizeInBytes{ 0 }	{}


	
	ImageAsset::ImageAsset(UniquePtr<unsigned char[]> rgbaData, size_t sizeInBytes)
		:
		sizeInBytes{ sizeInBytes },
		rgbaData{ std::move(rgbaData) }
	{
	}


	
	assetSystem::io::Archive &ImageAsset::Serialize(assetSystem::io::Archive &archive)
	{
		archive.Serialize("rgbaDataSize", reinterpret_cast<unsigned char *>(&sizeInBytes), 1, sizeof sizeInBytes);

		if(not rgbaData)
		{
			rgbaData = MakeUnique<unsigned char[]>(sizeInBytes);
		}

		return archive.Serialize("rgbaData", rgbaData.get(), sizeInBytes, sizeof(unsigned char));
		
	}


	
	const char *ImageAsset::GetAssetClassExtension()
	{
		static const char *extension{ "img" };

		return extension;
		
	}


	
	const wchar_t *ImageAsset::GetAssetClassExtensionW()
	{
		static const wchar_t *extension{ L"img" };

		return extension;
		
	}

	
}
