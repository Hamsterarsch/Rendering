#pragma once
#include "Renderer/Interface/Resources/SerializeTarget.hpp"
#include "AssetTypes/AssetBase.hpp"


namespace App::Assets
{	
	class CacheAsset final : public AssetBase<CacheAsset>
	{
		Renderer::SerializeTarget data;


		public: CacheAsset();

		public: explicit CacheAsset(int sizeInBytes);

		
		public: int GetSizeInBytes() const { return data.GetSizeInBytes(); }
		
		public: unsigned char *GetData() { return data.GetData(); }

		public: Renderer::SerializationHook &GetHook() { return data; }
							   		
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
				
	};
	
	
}
