#pragma once
#include "AssetTypes/AssetBase.hpp"
#include <string>


namespace App::Assets
{
	class ShaderAsset final : public AssetBase<ShaderAsset>
	{
		private: std::string shaderCode;



		public: ShaderAsset();

		public: ShaderAsset(const char *code) :  AssetBase{ GetAssetClassExtension() }, shaderCode{ code } {}


		
		public: const char *GetCode() const { return shaderCode.c_str(); }

		public: size_t GetShaderLength() const { return shaderCode.size(); }

		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
				
	};
	
	
}
