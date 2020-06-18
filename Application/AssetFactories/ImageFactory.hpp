#pragma once
#include "AssetTypes/ImageAsset.hpp"
#include "AssetFactories/ImageData.hpp"


namespace App::Assets
{
	class ImageAsset;

		
	class ImageFactory
	{
		public: static ImageAsset MakeImage(const char *absoluteImagePath);
							   		
		public: static ImageData LoadImageData(const char *absoluteImagePath);
		
	};

	
}
