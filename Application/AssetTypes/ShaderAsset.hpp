#pragma once
#include "Asset.hpp"
#include <string>


namespace App::Assets
{
	class ShaderAsset final : public assetSystem::Asset
	{
		private: std::string shaderCode;



		public: ShaderAsset();

		public: ShaderAsset(const char *code) : shaderCode{ code } {}


		
		public: const char *GetCode() const { return shaderCode.c_str(); }

		public: size_t GetShaderLength() const { return shaderCode.size(); }

		public: void SetCode(const std::string &code) { shaderCode = code; }

		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
				
	};
	
	
}
