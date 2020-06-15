#pragma once
#include "AssetSystem/Interface/Asset.hpp"
#include "Renderer/Interface/Resources/SerializationHook.hpp"


namespace App::Assets
{
	class CacheAsset final : public assetSystem::Asset
	{
		int dataSize{ 0 };
		
		UniquePtr<unsigned char[]> data;


		public: CacheAsset() = default;

		public: explicit CacheAsset(int sizeInBytes);

		
		public: int GetSize() const { return dataSize; }
		
		public: unsigned char *GetData() { return data.get(); }
							   		
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
				
	};
	
	
}
