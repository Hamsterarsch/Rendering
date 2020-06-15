#pragma once
#include "AssetSystem/Interface/Asset.hpp"
#include <string>


namespace App::Assets
{
	class ShaderAsset final : public assetSystem::Asset
	{
		private: std::string shaderCode;



		public: ShaderAsset() = default;

		public: ShaderAsset(const char *code) : shaderCode{ code } {}
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
		
		
	};
	
}
