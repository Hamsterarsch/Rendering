#include "AssetTypes/PipelineAsset.hpp"
#include "IO/Archive.hpp"


namespace App::Assets
{
	assetSystem::io::Archive &PipelineAsset::Serialize(assetSystem::io::Archive &archive)
	{
		using psoType_t = std::underlying_type_t<decltype(type)>;
		
		uint32_t typeContainer{ static_cast<psoType_t>(type) };
		archive.Serialize("pipelineType", typeContainer);
		type = static_cast<decltype(type)>(typeContainer);
				
		return archive
		.Serialize("pixelShader", pixelShader)
		.Serialize("vertexShader", vertexShader);
		
	}


	
	void PipelineAsset::UploadToRenderer(Renderer::RendererFacade &renderer)
	{
		if(psoHandle)
		{
			return;
			
		}
		
		MakeOpaquePipelineSettings(renderer);

		//add shader resource slots to signature here
		renderer.GetSignatureSettings()
		.AddConstant(1, 1)
		.DeclareTable()
		.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 1)
		.DeclareTable()
		.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 2, 1)
		.AddTableRange(&Renderer::DescriptorTargets::ShaderResource, 0, 5);
		
		Renderer::SerializeTarget root{};
		renderer.SerializeRootSignature(root, nullptr, 0);
		renderer.GetSignatureSettings().RestoreSettingsToSaved();
		
		//todo: cache serialized signature
		signatureHandle = { &renderer, renderer.MakeRootSignature(root.GetData(), root.GetSizeInBytes(), 0) };


		vertexShader->Compile(renderer);
		pixelShader->Compile(renderer);

		Renderer::ShaderList shaders{};
		shaders.vs.data = vertexShader->GetCompiledCode();
		shaders.vs.sizeInBytes = vertexShader->GetCompiledCodeSizeInBytes();
		shaders.ps.data = pixelShader->GetCompiledCode();
		shaders.ps.sizeInBytes = pixelShader->GetCompiledCodeSizeInBytes();

		psoHandle = { &renderer, renderer.MakePso(shaders, signatureHandle) };
		renderer.GetBlendSettings().RestoreSettingsToSaved();
		
	}


	
	void PipelineAsset::MakeOpaquePipelineSettings(Renderer::RendererFacade &renderer) const
	{
		renderer.GetBlendSettings().SetEnableBlend(false);
				
	}



	bool PipelineAsset::OccludesAllPixelsRenderedBehind() const
	{
		return type == PipelineType::Opaque;
		
	}



	const char *PipelineAsset::GetAssetClassExtension()
	{
		static const char *extension{ "pso" };

		return extension;
		
	}
	
}
