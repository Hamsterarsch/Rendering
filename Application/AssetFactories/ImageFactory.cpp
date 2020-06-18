#include "AssetFactories/ImageFactory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"


namespace App::Assets
{
	ImageAsset ImageFactory::MakeImage(const char *absoluteImagePath)
	{
		auto data{ LoadImageData(absoluteImagePath) };
		
		return ImageAsset{ std::move(data), absoluteImagePath };
		
	}

		ImageData ImageFactory::LoadImageData(const char* absoluteImagePath)
		{
			ImageData outData{};

			int width{ 0 };
			int height{ 0 };
			int originalComponentCount{ 0 };
			constexpr int forcedComponentCount{ 4 };
			
			auto *loadedData{ stbi_load(absoluteImagePath, &width, &height, &originalComponentCount, forcedComponentCount) };

			outData.width = static_cast<unsigned>(width);
			outData.height = static_cast<unsigned>(height);		
			outData.sizeInBytes = outData.width * outData.height * static_cast<unsigned>(forcedComponentCount);		
			outData.rgbaData = MakeUnique<unsigned char[]>(outData.sizeInBytes);
		
			std::memcpy(outData.rgbaData.get(), loadedData, outData.sizeInBytes);			
			stbi_image_free(loadedData);

			return outData;

		}

	
}
