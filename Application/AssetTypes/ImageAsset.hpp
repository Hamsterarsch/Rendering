#pragma once
#include "Renderer/Interface/Resources/HandleWrapper.hpp"
#include "AssetSystem/Interface/Asset.hpp"
#include "Shared/PtrTypes.hpp"
#include <string>


namespace App::Assets
{		
	class ImageAsset final : public assetSystem::Asset
	{
		private: Renderer::HandleWrapper textureHandle;

		private: std::string absoluteSourceImagePath;
		
		private: size_t sizeInBytes;
		
		private: UniquePtr<unsigned char[]> rgbaData;
		

		public: ImageAsset();

		public: ImageAsset(UniquePtr<unsigned char[]> &&rgbaData, size_t sizeInBytes, const char *absoluteSourceImagePath);


		void OnMakeAsset(const char* absoluteAssetFilePath) override;
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		void OnAssetLoaded(const char* absoluteAssetFilePath) override;
		
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();

	};

	
}
