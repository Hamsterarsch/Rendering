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
		const auto lastIncludePos{ templateCode.rfind("#include") };
		auto insertPos{ lastIncludePos == std::string::npos ? 0 : templateCode.find_first_of('\n', lastIncludePos)+1 };
		if(insertPos == std::string::npos)
		{
			insertPos = 0;
		}		
		templateCode.insert(insertPos, GetCode());			
		
		
		renderer.CompilePixelShader(templateCode.c_str(), templateCode.size(), outCode);
		
	}

	
}
