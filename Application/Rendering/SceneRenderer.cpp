#include "Rendering/SceneRenderer.hpp"
#include "Utility.hpp"
#include "AssetPtr.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "Shared/Debugging.hpp"
#include "AssetSystem.hpp"


namespace App::Rendering
{
	
	

	
	
	SceneRenderer::SceneRenderer
	(
		RendererMediator &mediator,
		Renderer::RendererFacade &renderer,
		assetSystem::AssetSystem &internalShaderProvider,
		const Math::VectorUint2 &surfaceDimensions
	)	:
		mediator{ &mediator },
		projection{ Math::Matrix::MakeProjection(Math::Radians(90), surfaceDimensions.x, surfaceDimensions.y, 100, 50'000) },
		createVolumeTileGridPipeline{ MakePsoVolumeTileGridCreationPipeline(internalShaderProvider, renderer) }
	{		
	}

		PipelineData SceneRenderer::MakePsoVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer)
		{						
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csCreateVolumeTileGrid{ shaderProvider.GetAsset("Shaders/CreateVolumeTileGrid.cs.shdr") };		
			csCreateVolumeTileGrid->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}					

		
			auto &settings{ renderer.GetSignatureSettings() };
			settings.RestoreSettingsToSaved();		
			settings
			.DeclareTable()
			.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 1)
			.AddTableRange(&Renderer::DescriptorTargets::UnorderedAccess, 0, 1);
		
			PipelineData out;		
			Renderer::SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };

		
			out.pso = { &renderer, renderer.MakePso(Renderer::Blob{csCreateVolumeTileGrid->GetCompiledCode(), csCreateVolumeTileGrid->GetCompiledCodeSizeInBytes()}, out.signature) };
		
			return out;
			
		}



	void SceneRenderer::SubmitFrame()
	{
	}


	
	void SceneRenderer::OnProjectionChanged()
	{
	}

	
}
