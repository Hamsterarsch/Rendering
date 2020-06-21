#pragma once
#include "AssetBase.hpp"
#include "Renderer/Interface/Resources/HandleWrapper.hpp"
#include "AssetFactories/ImageData.hpp"
#include <string>


namespace App::Assets
{
	struct ImageData;
	
	
	class ImageAsset final : public AssetBase<ImageAsset>
	{
		private: Renderer::HandleWrapper textureHandle;

		private: ImageData data;

		private: std::string absoluteSourceImagePath;
		
		
		public: ImageAsset();

		public: ImageAsset(ImageData &&data, const char *absoluteSourceImagePath);


		public: void OnMakeAsset(const char *absoluteAssetFilePath) override;
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: void OnAssetLoaded(const char *absoluteAssetFilePath) override;


		public: unsigned GetImageWidth() const { return data.width; }

		public: unsigned GetImageHeight() const { return data.height; }

		public: const unsigned char *GetImageData() const { return data.rgbaData.get(); }
		
		
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();

	};

	
}
