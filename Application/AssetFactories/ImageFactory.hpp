#pragma once
#include "AssetTypes/ImageAsset.hpp"



namespace App::Assets
{
	class ImageAsset;

		
	class ImageFactory
	{

		public: static ImageAsset MakeImage(const char *absoluteImagePath);

		public: struct ImageData
		{
			unsigned width;
			unsigned height;
			UniquePtr<unsigned char[]> rgbaData;
			size_t sizeInBytes;
		};
		
		public: static ImageData LoadImageData(const char *absoluteImagePath);
		
	};

	
}
