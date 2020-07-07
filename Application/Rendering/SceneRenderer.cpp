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
		pipelineCreateVolumeTileGrid{ MakeVolumeTileGridCreationPipeline(internalShaderProvider, renderer) },
		pipelineMarkActiveVolumeTiles{ MakeMarkActiveVolumeTilesPipeline(internalShaderProvider, renderer) },
		pipelineBuildActiveVolumeTileList{ MakeBuildActiveVolumeTileListPipeline(internalShaderProvider, renderer) }
	{
		
	}

		PipelineData SceneRenderer::MakeVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer)
		{						
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csCreateVolumeTileGrid{ shaderProvider.GetAsset("Shaders/CreateVolumeTileGrid.cs.shdr") };		
			csCreateVolumeTileGrid->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}					

		
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.DeclareTable()
			.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 1)
			.AddTableRange(&Renderer::DescriptorTargets::UnorderedAccess, 0, 1);
		
			PipelineData out;		
			Renderer::SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		

		
			out.pso = { &renderer, renderer.MakePso(Renderer::Blob{csCreateVolumeTileGrid->GetCompiledCode(), csCreateVolumeTileGrid->GetCompiledCodeSizeInBytes()}, out.signature) };
					
			return out;
		
		}

		PipelineData SceneRenderer::MakeMarkActiveVolumeTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer)
		{
			assetSystem::AssetPtrTyped<Assets::ShaderAsset>
				vsInstancing{ shaderProvider.GetAsset("Shaders/InstancedVertex.vs.shdr") },
				psMarkVolumeTiles{ shaderProvider.GetAsset("Shaders/MarkActiveVolumeTiles.ps.shdr") };

			vsInstancing->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}

			psMarkVolumeTiles->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}


			auto &settings{ renderer.GetSignatureSettings() };			
			settings
			.DeclareTable()
			.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 3)
			.AddTableRange(&Renderer::DescriptorTargets::UnorderedAccess, 1, 1);
								
			PipelineData out;		
			Renderer::SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		

		
			renderer.GetVertexLayoutSettings()
			.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetPosition, 0, &Renderer::FormatTargets::R32G32B32_Float, sizeof(float)*3);
		
			Renderer::ShaderList shaders;
			shaders.vs = {vsInstancing->GetCompiledCode(), vsInstancing->GetCompiledCodeSizeInBytes() };
			shaders.ps = { psMarkVolumeTiles->GetCompiledCode(), psMarkVolumeTiles->GetCompiledCodeSizeInBytes() };
		
			out.pso = { &renderer, renderer.MakePso(shaders, out.signature) };
			renderer.GetVertexLayoutSettings().RestoreSettingsToDefault();

			return out;		
		
		}

		PipelineData SceneRenderer::MakeBuildActiveVolumeTileListPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer)
		{
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csBuildActiveTileList{ shaderProvider.GetAsset("Shaders/BuildActiveTileList.cs.shdr") };		
			csBuildActiveTileList->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}					

		
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.DeclareTable()
			.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 1)
			.AddTableRange(&Renderer::DescriptorTargets::ShaderResource, 0, 1)
			.AddTableRange(&Renderer::DescriptorTargets::UnorderedAccess, 0, 1);
		
			PipelineData out;		
			Renderer::SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		
					
			out.pso = { &renderer, renderer.MakePso(Renderer::Blob{csBuildActiveTileList->GetCompiledCode(), csBuildActiveTileList->GetCompiledCodeSizeInBytes()}, out.signature) };
					
			return out;
		
		}



	void SceneRenderer::SubmitFrame()
	{
	}


	
	void SceneRenderer::OnProjectionChanged()
	{
	}

	
}
