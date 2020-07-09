#pragma once
#include "AssetTypes/ShaderAsset.hpp"
#include "AssetPtr.hpp"


namespace App::Assets
{
	class UserPixelShaderAsset final : public ShaderAsset
	{
		private: static assetSystem::AssetPtrTyped<ShaderAsset> pixelShaderTemplate;



		public: UserPixelShaderAsset();

		
		private: void CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode) override;
		
		public: static const char *GetAssetClassExtension()
		{						
			static const std::string extension{ std::string{"ups."} + ShaderAsset::GetAssetClassExtension() };

			return extension.c_str();
			
		}
				
		public: static void SetPixelShaderTemplate(const assetSystem::AssetPtrTyped<ShaderAsset> &shaderTemplate) { pixelShaderTemplate = shaderTemplate; }
		
	};

	
}
