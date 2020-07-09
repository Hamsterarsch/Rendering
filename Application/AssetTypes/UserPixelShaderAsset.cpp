#include "AssetTypes/UserPixelShaderAsset.hpp"


namespace App::Assets
{
	assetSystem::AssetPtrTyped<ShaderAsset> UserPixelShaderAsset::pixelShaderTemplate{};



	UserPixelShaderAsset::UserPixelShaderAsset()
		:
		ShaderAsset	{ "float3 DiffuseColor(LightingV2P IN)\n{\n\treturn float3(.5,.5,.5);\n}" }
	{
	}

	

	void UserPixelShaderAsset::CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode)
	{
		std::string templateCode{ pixelShaderTemplate->GetCode() };
		const auto lastIncludePos{ templateCode.find_last_of("#include") };
		const auto includeEndPos{ templateCode.find_first_of('\n', lastIncludePos) };

		templateCode.insert(includeEndPos+1, GetCode());		
		
		renderer.CompilePixelShader(templateCode.c_str(), templateCode.size(), outCode);
		
	}

	
}
