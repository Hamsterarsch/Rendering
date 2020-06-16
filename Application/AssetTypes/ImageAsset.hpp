#pragma once
#include "Renderer/Interface/Resources/HandleWrapper.hpp"
#include "AssetSystem/Interface/Asset.hpp"
#include "Shared/PtrTypes.hpp"


namespace App::Assets
{
	class ImageAsset final : public assetSystem::Asset
	{
		private: Renderer::HandleWrapper textureHandle;

		private: size_t sizeInBytes;
		
		private: UniquePtr<unsigned char[]> rgbaData;
		

		public: ImageAsset();

		public: ImageAsset(UniquePtr<unsigned char[]> rgbaData, size_t sizeInBytes);


		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
				
	};
	
	
}
