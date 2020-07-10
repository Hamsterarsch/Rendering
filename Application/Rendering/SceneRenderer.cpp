#include "Rendering/SceneRenderer.hpp"
#include "Utility.hpp"
#include "AssetPtr.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "Shared/Debugging.hpp"
#include "AssetSystem.hpp"
#include "VolumeTileGrid.hpp"
#include "Commands/CompositeCommand.hpp"


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
		projection{},
		pipelineCreateVolumeTileGrid{ MakeVolumeTileGridCreationPipeline(internalShaderProvider, renderer) },
		pipelineMarkActiveVolumeTiles{ MakeMarkActiveVolumeTilesPipeline(internalShaderProvider, renderer) },
		pipelineBuildActiveVolumeTileList{ MakeBuildActiveVolumeTileListPipeline(internalShaderProvider, renderer) },
		pipelineAssignLightsToTiles{ MakeAssignLightsToTilesPipeline(internalShaderProvider, renderer) },
		commandFactory{ renderer.MakeCommandFactory() }
	{		
		VolumeTileGridData gridData{};
		gridData.nearDistance = 100;
		gridData.farDistance = 50'000;
		gridData.screenDimensions = surfaceDimensions;
		
		projection = Math::Matrix::MakeProjection(Math::Radians(90), surfaceDimensions.x, surfaceDimensions.y, gridData.nearDistance, gridData.farDistance);
		gridData.inverseProjection = projection.Inverse();
		
		VolumeTileGrid grid{ {64, 64}, 90.f, gridData };
		gridDataBuffer = { &renderer, renderer.MakeBuffer(&gridData, sizeof gridData) };
		gridBoundingBoxBuffer = { &renderer, renderer.MakeUavBuffer(nullptr, grid.SizeInBytes()) };

		Renderer::HandleWrapper view
		{
			&renderer,
			renderer.GetViewFactory()
			.DeclareSpecificDescriptorBlock(pipelineCreateVolumeTileGrid.signature, 0, 2, 0)
			.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, sizeof gridData)
			.DeclareOrdinal(1).CreateUnorderedAccessView(gridBoundingBoxBuffer, 0, grid.GetTileCount(), grid.GetTileStride())
			.FinalizeDescriptorBlock()
		};

		auto createGridCommand{ MakeUnique<Renderer::Commands::CompositeCommand>() };
		createGridCommand->Add(commandFactory->SetSignatureCompute(pipelineCreateVolumeTileGrid.signature));
		createGridCommand->Add(commandFactory->SetPipelineState(pipelineCreateVolumeTileGrid.pso));
		createGridCommand->Add(commandFactory->SetDescriptorBlockViewsAsComputeTable(view, 0));
		createGridCommand->Add(commandFactory->Dispatch
		(
			std::ceil(gridData.outGridDimensions.x / 4.f),
			std::ceil(gridData.outGridDimensions.y / 4.f),
			std::ceil(gridData.outGridDimensions.z / 4.f)
		));

		renderer.SubmitCommand(std::move(createGridCommand));
		
		auto counter{ renderer.GetCounterFactory().MakeCounter(0) };
		renderer.SubmitCommand(commandFactory->IncreaseCounter(counter, 1));
		renderer.GetCounterFactory().WaitForCounterToReach(counter, 1);
				
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

		PipelineData SceneRenderer::MakeAssignLightsToTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer)
		{
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csBuildActiveTileList{ shaderProvider.GetAsset("Shaders/AssignLightsToTiles.cs.shdr") };		
			csBuildActiveTileList->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}					

		
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.DeclareTable()
			.AddTableRange(&Renderer::DescriptorTargets::ConstantBuffer, 0, 2)
			.AddTableRange(&Renderer::DescriptorTargets::ShaderResource, 0, 3)
			.AddTableRange(&Renderer::DescriptorTargets::UnorderedAccess, 0, 2);
		
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
