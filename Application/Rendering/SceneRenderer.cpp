#include "Rendering/SceneRenderer.hpp"
#include "Utility.hpp"
#include "AssetPtr.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "Shared/Debugging.hpp"
#include "AssetSystem.hpp"
#include "Commands/CompositeCommand.hpp"
#include "Rendering/GraphVisitorHarvestMeshes.hpp"
#include "RendererMediator.hpp"
#include "ContextCommands/ContextCompositeCommand.hpp"


namespace App::Rendering
{
	SceneRenderer::SceneRenderer() = default;

	

	SceneRenderer::SceneRenderer
	(
		RendererMediator &mediator,
		RendererFacade &renderer,
		assetSystem::AssetSystem &internalShaderProvider,
		const Math::VectorUint2 &surfaceDimensions
	)	:
		mediator{ &mediator },
		pipelineDepthOnly{ MakeDepthOnlyPipeline(internalShaderProvider, renderer) },
		pipelineCreateVolumeTileGrid{ MakeVolumeTileGridCreationPipeline(internalShaderProvider, renderer) },
		pipelineMarkActiveVolumeTiles{ MakeMarkActiveVolumeTilesPipeline(internalShaderProvider, renderer) },
		pipelineBuildActiveVolumeTileList{ MakeBuildActiveVolumeTileListPipeline(internalShaderProvider, renderer) },
		pipelineAssignLightsToTiles{ MakeAssignLightsToTilesPipeline(internalShaderProvider, renderer) },
		commandFactory{ renderer.MakeCommandFactory() },
		volumeTileGridIsInvalidated{ false }
	{		
		UpdateProjection(surfaceDimensions);
		SubmitCommandsToFitVolumeTileGridToProjection(renderer);
		SetCamera({0, 0, -2}, {0,0,0});
	}

		PipelineData SceneRenderer::MakeDepthOnlyPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{
			Assets::StaticMeshData::ConfigureVertexLayoutSettings(renderer.GetVertexLayoutSettings());//todo: this should not be done in this function. Also, what about different vertex layouts
			renderer.GetVertexLayoutSettings().SaveSettings();
			renderer.GetRasterizerSettings().SetFrontIsCounterClockwise(false).SaveSettings();
		
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> vsInstancing{ shaderProvider.GetAsset("Shaders/InstancedVertex.vs.shdr") };
				
			vsInstancing->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}

			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.AddConstant(1, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 2);
			
					
			PipelineData out;		
			SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		


			ShaderList shaders{};
			shaders.vs.sizeInBytes = vsInstancing->GetCompiledCodeSizeInBytes();
			shaders.vs.data = vsInstancing->GetCompiledCode();		
			out.pso = { &renderer, renderer.MakePso(shaders, out.signature) };
					
			return out;
					
		}

		PipelineData SceneRenderer::MakeVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
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
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 1);
		
			PipelineData out;		
			SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		

		
			out.pso = { &renderer, renderer.MakePso(Blob{csCreateVolumeTileGrid->GetCompiledCode(), csCreateVolumeTileGrid->GetCompiledCodeSizeInBytes()}, out.signature) };
					
			return out;
		
		}

		PipelineData SceneRenderer::MakeMarkActiveVolumeTilesPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
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
			.AddConstant(1, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 2)
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 2, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 1, 1);
								
			PipelineData out;		
			SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		

		
			ShaderList shaders;
			shaders.vs = {vsInstancing->GetCompiledCode(), vsInstancing->GetCompiledCodeSizeInBytes() };
			shaders.ps = { psMarkVolumeTiles->GetCompiledCode(), psMarkVolumeTiles->GetCompiledCodeSizeInBytes() };
		
			out.pso = { &renderer, renderer.MakePso(shaders, out.signature) };
			
			return out;		
		
		}

		PipelineData SceneRenderer::MakeBuildActiveVolumeTileListPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csBuildActiveTileList{ shaderProvider.GetAsset("Shaders/BuildActiveTileList.cs.shdr") };		
			csBuildActiveTileList->Compile(renderer);
			if(not renderer.WasCompileSuccessful())
			{
				Exception::DebugBreak();
			}					

		
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.AddConstant(0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 1);
		
			PipelineData out;		
			SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		
					
			out.pso = { &renderer, renderer.MakePso(Blob{csBuildActiveTileList->GetCompiledCode(), csBuildActiveTileList->GetCompiledCodeSizeInBytes()}, out.signature) };
					
			return out;
		
		}

		PipelineData SceneRenderer::MakeAssignLightsToTilesPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
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
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 1, 1)
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 3)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 2);
		
			PipelineData out;		
			SerializeTarget rootData;
			renderer.SerializeRootSignature(rootData, nullptr, 0);
			out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
			settings.RestoreSettingsToSaved();		
					
			out.pso = { &renderer, renderer.MakePso(Blob{csBuildActiveTileList->GetCompiledCode(), csBuildActiveTileList->GetCompiledCodeSizeInBytes()}, out.signature) };
					
			return out;
					
		}

		void SceneRenderer::UpdateProjection(const Math::VectorUint2 &screenDimensions)
		{
			globalData.projection = Math::Matrix::MakeProjection(Math::Radians(verticalFovDegrees), screenDimensions.x, screenDimensions.y, nearDistance, farDistance);
			globalData.inverseProjection = globalData.projection.Inverse();
			globalData.screenResolution.x = screenDimensions.x;
			globalData.screenResolution.y = screenDimensions.y;
					
		}

		void SceneRenderer::SubmitCommandsToFitVolumeTileGridToProjection(RendererFacade &renderer)
		{
			VolumeTileGridData gridData{};
			gridData.nearDistance = nearDistance;
			gridData.farDistance = farDistance;
			gridData.screenDimensions = globalData.screenResolution;				
			gridData.inverseProjection = globalData.inverseProjection;
			
			
			currentVolumeTileGrid = { {128, 128}, verticalFovDegrees, tileDepthScale, gridData };
			gridDataBufferSizeInBytes = sizeof gridData;
			gridDataBuffer = { &renderer, renderer.MakeBuffer(&gridData, gridDataBufferSizeInBytes) };
			gridBoundingBoxBuffer = { &renderer, renderer.MakeUaBuffer(nullptr, currentVolumeTileGrid.SizeInBytes()) };

			HandleWrapper view
			{
				&renderer,
				renderer.GetViewFactory()
				.DeclareSpecificDescriptorBlock(pipelineCreateVolumeTileGrid.signature, 0, 2, 0)
				.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, sizeof gridData)
				.DeclareOrdinal(1).CreateUnorderedAccessView(gridBoundingBoxBuffer, 0, currentVolumeTileGrid.GetTileCount(), currentVolumeTileGrid.GetTileStride())
				.FinalizeDescriptorBlock()
			};

			auto createGridCommand{ MakeUnique<Commands::CompositeCommand>() };
			createGridCommand->Add(commandFactory->SetSignatureCompute(pipelineCreateVolumeTileGrid.signature));
			createGridCommand->Add(commandFactory->SetPipelineState(pipelineCreateVolumeTileGrid.pso));
			createGridCommand->Add(commandFactory->SetDescriptorBlockViewsAsComputeTable(view, 0));
			createGridCommand->Add(commandFactory->Dispatch
			(
				std::ceil(gridData.outGridDimensions.x / 4.f),
				std::ceil(gridData.outGridDimensions.y / 4.f),
				std::ceil(gridData.outGridDimensions.z / 4.f)
			));
			createGridCommand->Add(commandFactory->TransitionUnorderedAccessToShaderResource(gridBoundingBoxBuffer, true));

			renderer.SubmitCommand(std::move(createGridCommand));
			volumeTileGridIsInvalidated = false;
		
		}


	
	SceneRenderer::SceneRenderer(SceneRenderer &&) noexcept = default;

	SceneRenderer &SceneRenderer::operator=(SceneRenderer &&) noexcept = default;
	
	SceneRenderer::~SceneRenderer() = default;



	
	void SceneRenderer::SubmitFrame(const UniquePtr<GraphVisitorHarvestMeshes> &graphData)
	{
		if(volumeTileGridIsInvalidated)
		{
			SubmitCommandsToFitVolumeTileGridToProjection(mediator->Renderer());
		}
		
		if(graphData->meshInstanceData.empty())
		{
			return;
		}
		
		UploadGlobalBuffer(globalData.time + .1f, mediator->Renderer());//todo provide proper time

		auto &cmds{ mediator->CommandFactory() };

		
		//render depth pre
		auto bindDepthContext{ MakeUnique<ContextCompositeCommand>(Commands::CommandContextEventFlags::AllBindingsInvalidated | Commands::CommandContextEventFlags::InitialContextCommandExecution) };
		bindDepthContext->Add(cmds.BindDepthTargetOnly(mediator->GetMainDepthTextureView()));
		mediator->Renderer().SubmitContextCommand(std::move(bindDepthContext));
		
		auto cmd{ MakeUnique<Commands::CompositeCommand>() };
		cmd->Add(cmds.SetViewport(0, 0, globalData.screenResolution.x, globalData.screenResolution.y, 0, 1));
		cmd->Add(cmds.SetScissorRect(0, 0, globalData.screenResolution.x, globalData.screenResolution.y));
		
		cmd->Add(cmds.SetPipelineState(pipelineDepthOnly.pso));
		cmd->Add(cmds.SetSignatureGraphics(pipelineDepthOnly.signature));
		cmd->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(globalDataBufferDescriptor, 1));

		currentTransformData.clear();
		currentTransformIndexData.clear();
		currentDrawCommands = MakeUnique<Commands::CompositeCommand>();
		std::vector<uint32_t> offsetToMeshTransforms{};
		for(auto &&perMeshData : graphData->meshInstanceData)
		{			
			//bind mesh todo: only do this for meshes that are really rendered
			auto mesh{ perMeshData.first };
			AddMeshBindingToCurrentCommands(mesh);
			
			for(unsigned shardIndex{ 0 }; shardIndex < perMeshData.second.meshShardData.size(); ++shardIndex)
			{
				auto &shardData{ perMeshData.second.meshShardData.at(shardIndex) };				
				for(auto &&shardPipelineOccurence : shardData.pipelineOccurences)
				{
					if(shardPipelineOccurence.first->OccludesAllPixelsRenderedBehind())
					{
						const auto numInstances{ shardPipelineOccurence.second.data.size() };						
						AddMeshShardDrawToCurrentCommands(mesh->GetMeshShardInfo(shardIndex), numInstances, shardPipelineOccurence.second.data, currentTransformData.size());															
					}
				}
				
			}
			offsetToMeshTransforms.emplace_back(static_cast<uint32_t>(currentTransformData.size()));
			AppendMeshTransformsToTransformData(perMeshData.second.transforms);
			
		}
		
		//upload shard transforms
		const auto transformStride{ sizeof(decltype(currentTransformData)::value_type) };
		const auto transformSizeInBytes{ currentTransformData.size()*transformStride };
		HandleWrapper meshTransforms{ &mediator->Renderer(), mediator->Renderer().MakeBuffer( currentTransformData.data(), transformSizeInBytes ) };
		{
		const auto transformIndexStride{ sizeof(decltype(currentTransformIndexData)::value_type) };
		const auto transformIndicesSizeInBytes{ currentTransformIndexData.size()*transformIndexStride };
		HandleWrapper indicesIntoTransforms{ &mediator->Renderer(), mediator->Renderer().MakeBuffer( currentTransformIndexData.data(), transformIndicesSizeInBytes ) };
		HandleWrapper transformTableContent
		{
			&mediator->Renderer(),
			mediator->Renderer().GetViewFactory()
			.DeclareDescriptorBlock(2, 0)
			.CreateShaderResourceView(meshTransforms, 0, currentTransformData.size(), transformStride)
			.CreateShaderResourceView(indicesIntoTransforms, 0, currentTransformIndexData.size(), transformIndexStride)
			.FinalizeDescriptorBlock()
		};
		
		//submit tranform bind
		cmd->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(transformTableContent, 2));
		
		//submit draw commands
		cmd->Add(std::move(currentDrawCommands));
		mediator->Renderer().SubmitCommand(std::move(cmd));
		}
		

		
		//mark active tiles
		//assign lights to active tiles todo: reduce relevant light set before this step
		auto markTilesCommand{ MakeUnique<Commands::CompositeCommand>() };
		markTilesCommand->Add(cmds.SetPipelineState(pipelineMarkActiveVolumeTiles.pso));
		markTilesCommand->Add(cmds.SetSignatureGraphics(pipelineMarkActiveVolumeTiles.signature));
		markTilesCommand->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(globalDataBufferDescriptor, 1));
		
		currentTransformIndexData.clear();
		currentDrawCommands = MakeUnique<Commands::CompositeCommand>();

		
		uint32_t meshIndex{ 0 };
		for(auto &&perMeshData : graphData->meshInstanceData)
		{			
			//bind mesh todo: only do this for meshes that are really rendered
			auto mesh{ perMeshData.first };
			AddMeshBindingToCurrentCommands(mesh);
			
			for(unsigned shardIndex{ 0 }; shardIndex < perMeshData.second.meshShardData.size(); ++shardIndex)
			{
				auto &shardData{ perMeshData.second.meshShardData.at(shardIndex) };				
				for(auto &&shardPipelineOccurence : shardData.pipelineOccurences)
				{
					auto pipeline{ shardPipelineOccurence.first };
					pipeline->UploadToRenderer(mediator->Renderer());
					
					const auto numInstances{ shardPipelineOccurence.second.data.size() };						
					AddMeshShardDrawToCurrentCommands(mesh->GetMeshShardInfo(shardIndex), numInstances, shardPipelineOccurence.second.data, offsetToMeshTransforms.at(meshIndex));																				
				}				
			}
			++meshIndex;
		}
		
		const auto transformIndexStride{ sizeof(decltype(currentTransformIndexData)::value_type) };
		auto transformIndicesSizeInBytes{ currentTransformIndexData.size()*transformIndexStride };
		HandleWrapper indicesIntoTransforms{ &mediator->Renderer(), mediator->Renderer().MakeBuffer( currentTransformIndexData.data(),  transformIndicesSizeInBytes) };
		
		HandleWrapper volumeTileFlags{ &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, currentVolumeTileGrid.GetTileCount() * sizeof(uint32_t)) };
				
		HandleWrapper tableContent
		{
			&mediator->Renderer(),
			mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineMarkActiveVolumeTiles.signature, 1, 4, 0)
			.DeclareOrdinal(1).CreateShaderResourceView(meshTransforms, 0, currentTransformData.size(), transformStride)
			.DeclareOrdinal(2).CreateShaderResourceView(indicesIntoTransforms, 0, currentTransformIndexData.size(), transformIndexStride)
			.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
			.DeclareOrdinal(1).CreateUnorderedAccessView(volumeTileFlags, 0, currentVolumeTileGrid.GetTileCount(), sizeof(uint32_t))
			.FinalizeDescriptorBlock()			
		};
		markTilesCommand->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(tableContent, 2));
		markTilesCommand->Add(std::move(currentDrawCommands));		
		mediator->Renderer().SubmitCommand(std::move(markTilesCommand));
		
		//build active tile list
		auto buildTileListCmd{ MakeUnique<Commands::CompositeCommand>() };
		buildTileListCmd->Add(cmds.SetPipelineState(pipelineBuildActiveVolumeTileList.pso));
		buildTileListCmd->Add(cmds.SetSignatureCompute(pipelineBuildActiveVolumeTileList.signature));
		buildTileListCmd->Add(cmds.SetComputeConstants(0, currentVolumeTileGrid.GetTileCount(), 1, 0));
		buildTileListCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(volumeTileFlags, false));

		const auto activeTileListElementCount{ currentVolumeTileGrid.GetTileCount()+1 };
		const auto activeTileListStride{ sizeof(uint32_t) };
		HandleWrapper activeTileList{ &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, activeTileListElementCount*activeTileListStride) };
		
		HandleWrapper resourceViews
		{
			&mediator->Renderer(),
			mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineBuildActiveVolumeTileList.signature, 0, 2, 0)
			.DeclareOrdinal(1).CreateShaderResourceView(volumeTileFlags, 0, currentVolumeTileGrid.GetTileCount(), sizeof(uint32_t))
			.DeclareOrdinal(1).CreateUnorderedAccessView(activeTileList, 0, activeTileListElementCount, activeTileListStride)
			.FinalizeDescriptorBlock()
		};

		buildTileListCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(resourceViews, 1));
		buildTileListCmd->Add(cmds.Dispatch(std::ceil(currentVolumeTileGrid.GetTileCount() / 32.f), 1, 1));

		mediator->Renderer().SubmitCommand(std::move(buildTileListCmd));
				
		uint32_t numActiveTiles{ 0 };
		mediator->Renderer().QueryUaResourceContent(activeTileList, sizeof(numActiveTiles), &numActiveTiles);

		if(numActiveTiles <= 0)
		{
			return;
			
		}

		HandleWrapper globalLightData{};
		HandleWrapper activeLightIndicesForTiles{};
		HandleWrapper tileToLightIndexMap{};
		const auto lightBufferStride{ sizeof(decltype(graphData->lightData)::value_type) };

		const auto activeLightIndicesElementCount{ numActiveTiles*graphData->lightData.size() };
		const auto activeLightIndicesElementStride{ sizeof(uint16_t) };

		const auto tileToLightMapStride{ sizeof(LightIndexListInfo) };

		if(not graphData->lightData.empty())		
		{
			globalLightData = { &mediator->Renderer(), mediator->Renderer().MakeBuffer( graphData->lightData.data(), graphData->lightData.size() * lightBufferStride) };			
			activeLightIndicesForTiles = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, activeLightIndicesElementCount*activeLightIndicesElementStride) };
			tileToLightIndexMap = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, currentVolumeTileGrid.GetTileCount()*tileToLightMapStride) };
					
			HandleWrapper assignLightsTable
			{
				&mediator->Renderer(),
				mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineAssignLightsToTiles.signature, 1, 6, 0)
				.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
				.DeclareOrdinal(1).CreateShaderResourceView(globalLightData, 0, graphData->lightData.size(), lightBufferStride)
				.DeclareOrdinal(2).CreateShaderResourceView(gridBoundingBoxBuffer, 0, currentVolumeTileGrid.GetTileCount(), currentVolumeTileGrid.GetTileStride())
				.DeclareOrdinal(3).CreateShaderResourceView(activeTileList, 0, activeTileListElementCount, activeTileListStride)
				.DeclareOrdinal(1).CreateUnorderedAccessView(activeLightIndicesForTiles, 0, activeLightIndicesElementCount, activeLightIndicesElementStride)
				.DeclareOrdinal(2).CreateUnorderedAccessView(tileToLightIndexMap, 0, currentVolumeTileGrid.GetTileCount(), tileToLightMapStride)
				.FinalizeDescriptorBlock()
			};

			auto assignLightsCmd{ MakeUnique<Commands::CompositeCommand>() };
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(activeTileList, false));
			assignLightsCmd->Add(cmds.SetPipelineState(pipelineAssignLightsToTiles.pso));
			assignLightsCmd->Add(cmds.SetSignatureCompute(pipelineAssignLightsToTiles.signature));
			assignLightsCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(globalDataBufferDescriptor, 0));
			assignLightsCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(assignLightsTable, 1));
			assignLightsCmd->Add(cmds.Dispatch(std::ceil(numActiveTiles /32.f), 1, 1));
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(activeLightIndicesForTiles, true));
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(tileToLightIndexMap, true));
						
			mediator->Renderer().SubmitCommand(std::move(assignLightsCmd));			
		}
		
		//render with user pixel
		currentDrawCommands = MakeUnique<Commands::CompositeCommand>();
		currentTransformIndexData.clear();
		meshIndex = 0;
				
		for(auto &&perMeshData : graphData->meshInstanceData)
		{			
			//bind mesh todo: only do this for meshes that are really rendered
			auto mesh{ perMeshData.first };
			AddMeshBindingToCurrentCommands(mesh);
			
			for(unsigned shardIndex{ 0 }; shardIndex < perMeshData.second.meshShardData.size(); ++shardIndex)
			{
				auto &shardData{ perMeshData.second.meshShardData.at(shardIndex) };				
				for(auto &&shardPipelineOccurence : shardData.pipelineOccurences)
				{
					currentDrawCommands->Add(cmds.SetPipelineState(shardPipelineOccurence.first->GetPsoHandle()));
					currentDrawCommands->Add(cmds.SetSignatureGraphics(shardPipelineOccurence.first->GetSignatureHandle()));
					//currentDrawCommands->Add(cmds.) bind per instance data here
					
					const auto numInstances{ shardPipelineOccurence.second.data.size() };						
					AddMeshShardDrawToCurrentCommands(mesh->GetMeshShardInfo(shardIndex), numInstances, shardPipelineOccurence.second.data, offsetToMeshTransforms.at(meshIndex));																				
				}				
			}
			++meshIndex;
		}

		
		transformIndicesSizeInBytes = transformIndexStride * currentTransformIndexData.size();
		HandleWrapper meshTransformIndices{ &mediator->Renderer(), mediator->Renderer().MakeBuffer(currentTransformIndexData.data(), transformIndicesSizeInBytes) };
		
		//add context command binding all static resources/
		HandleWrapper lightingPassTable
		{
			&mediator->Renderer(),
			mediator->Renderer().GetViewFactory().DeclareDescriptorBlock(6, 0)
			.CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
			.CreateShaderResourceView(meshTransforms, 0, currentTransformData.size(), transformStride)
			.CreateShaderResourceView(meshTransformIndices, 0, currentTransformIndexData.size(), transformIndexStride)
			.CreateShaderResourceView(globalLightData, 0, graphData->lightData.size(), lightBufferStride)
			.CreateShaderResourceView(activeLightIndicesForTiles, 0, activeLightIndicesElementCount, activeLightIndicesElementStride)
			.CreateShaderResourceView(tileToLightIndexMap, 0, currentVolumeTileGrid.GetTileCount(), tileToLightMapStride)
			.FinalizeDescriptorBlock()
		};

		auto contextCmds{ MakeUnique<Commands::CompositeCommand>() };

		auto contextCmdTargetBind( MakeUnique<ContextCompositeCommand>(Commands::CommandContextEventFlags::AllBindingsInvalidated | Commands::CommandContextEventFlags::InitialContextCommandExecution));
		contextCmdTargetBind->Add(cmds.BindRenderTargets(mediator->GetMainWindowSurface(), mediator->GetMainDepthTextureView()));
		contextCmds->Add(std::move(contextCmdTargetBind));
		
		auto contextCmdResourceBind{ MakeUnique<ContextCompositeCommand>(Commands::CommandContextEventFlags::GraphicsBindingsInvalidated) };
		contextCmdResourceBind->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(globalDataBufferDescriptor, 1));
		contextCmdResourceBind->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(lightingPassTable, 2));
		contextCmds->Add(std::move(contextCmdResourceBind));
		
		mediator->Renderer().SubmitContextCommand(std::move(contextCmds));
		mediator->Renderer().SubmitCommand(std::move(currentDrawCommands));
		mediator->Renderer().SubmitDefaultContextCommand();

				
		/*
		auto c{ mediator->Renderer().GetCounterFactory().MakeCounter(0) };
		mediator->Renderer().SubmitCommand(cmds.IncreaseCounter(c, 1));
		mediator->Renderer().GetCounterFactory().WaitForCounterToReach(c, 1);
	*/	
		
	}

		void SceneRenderer::UploadGlobalBuffer(const float newTime, RendererFacade &renderer)
		{
			globalData.time = newTime;
			globalDataBuffer = { &renderer, renderer.MakeBuffer(&globalData, sizeof globalData) };
			globalDataBufferDescriptor = 
			{
				&renderer,
				renderer.GetViewFactory()
				.DeclareDescriptorBlock(1, 0)
				.CreateConstantBufferView(globalDataBuffer, sizeof globalData)
				.FinalizeDescriptorBlock()
			};
		
		}

		void SceneRenderer::AddMeshBindingToCurrentCommands(assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> &mesh)
		{
			mesh->UploadToRenderer(mediator->Renderer());
							
			currentDrawCommands->Add(mediator->CommandFactory().SetVertexBuffer
			(
				mesh->GetMeshBuffer(),
				0,
				mesh->GetMeshBufferVertexCount(),
				mesh->GetVertexStrideInBytes()
			));

			currentDrawCommands->Add(mediator->CommandFactory().SetIndexBuffer
			(
				mesh->GetMeshBuffer(),
				mesh->GetByteOffsetToIndexData(),
				mesh->GetNumIndices(),
				mesh->GetIndexSizeInBytes(),
				mesh->GetIndexFormat()
			));
		
		}

		void SceneRenderer::AddMeshShardDrawToCurrentCommands(const Assets::MeshSubgroup &shardInfo, const unsigned numInstances, std::vector<MeshShardInstanceData> &instanceData, uint32_t offsetToMeshTransforms)
		{
			const auto offsetToTransformIndices{ currentTransformIndexData.size() };
			for(auto &&shardInstance : instanceData)
			{
				currentTransformIndexData.emplace_back(offsetToMeshTransforms + shardInstance.shardTransformIndex);
			}
						
			currentDrawCommands->Add(mediator->CommandFactory().SetGraphicConstants(0, offsetToTransformIndices, 1, 0));
						
			currentDrawCommands->Add(mediator->CommandFactory().DrawIndexedInstanced
			(
				numInstances,
				shardInfo.numIndices,
				shardInfo.offsetToIndices,
				0
			));
		
		}


		void SceneRenderer::AppendMeshTransformsToTransformData(const std::vector<Math::Matrix> &meshTransforms)
		{
			for(auto &&transform : meshTransforms)
			{				
				currentTransformData.emplace_back(transform);						
			}
		
		}
	
	
	void SceneRenderer::SetCamera(const Math::Vector3 &position, const Math::Vector3 &rotation)
	{
		globalData.view = Math::Matrix::MakeRotation(-rotation.x, -rotation.y, -rotation.z)*Math::Matrix::MakeTranslation(-position.x, -position.y, -position.z);
		globalData.inverseView = globalData.view.Inverse();
		
	}



	void SceneRenderer::OnMainWindowSurfaceSizeChanged(const Math::VectorUint2 &newSize)
	{
		UpdateProjection(newSize);
		volumeTileGridIsInvalidated = true;
						
	}

	
}
