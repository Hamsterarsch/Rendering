#include "AssetTypes/ImageAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"
#include <filesystem>
#include "AssetFactories/ImageFactory.hpp"


namespace App::Assets
{
	ImageAsset::ImageAsset() : data{} {}


	
	ImageAsset::ImageAsset(ImageData &&data, const char *absoluteSourceImagePath)
		:
		data{ std::move(data) },
		absoluteSourceImagePath{ absoluteSourceImagePath }
	{
	}


	
	void ImageAsset::OnMakeAsset(const char *absoluteAssetFilePath)
	{
		std::filesystem::path srcImagePath{ absoluteSourceImagePath };
		const auto imageExtension{ srcImagePath.extension() };

		std::filesystem::path dstImagePath{ absoluteAssetFilePath };
		dstImagePath.replace_extension().replace_extension(imageExtension);
		
		auto r{ copy_file(srcImagePath, dstImagePath) };			
				
	}


	
	assetSystem::io::Archive &ImageAsset::Serialize(assetSystem::io::Archive &archive)
	{		
		return archive;
		
	}


	void ImageAsset::OnAssetLoaded(const char *absoluteAssetFilePath)
	{
		std::filesystem::path path{ absoluteAssetFilePath };
		path.replace_extension().replace_extension("png");
		data = ImageFactory::LoadImageData(path.string().c_str());
				
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
