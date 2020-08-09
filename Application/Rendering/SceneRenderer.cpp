#include "Rendering/SceneRenderer.hpp"
#include "Utility.hpp"
#include "AssetPtr.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "AssetSystem.hpp"
#include "Commands/CompositeCommand.hpp"
#include "Rendering/GraphVisitorHarvestMeshes.hpp"
#include "RendererMediator.hpp"
#include "ContextCommands/ContextCompositeCommand.hpp"


namespace App::Rendering
{	
	struct BufferInfo
	{
		size_t elementStrideInBytes;
		size_t numElements;
		size_t sizeInBytes;
		HandleWrapper handle;


		BufferInfo() : elementStrideInBytes{ 0 }, numElements{ 0 }, sizeInBytes{ 0 } {};
		
		template<class t>
		BufferInfo(RendererFacade &renderer, const std::vector<t> &data)
			:
			elementStrideInBytes{ sizeof(std::remove_reference_t<decltype(data)>::value_type) },
			numElements{ data.size() },
			sizeInBytes{ elementStrideInBytes*numElements },
			handle{ &renderer, renderer.MakeBuffer(data.data(), elementStrideInBytes*sizeInBytes) }
		{}

		BufferInfo(size_t numElements, size_t elementStrideInBytes) : elementStrideInBytes{ elementStrideInBytes }, numElements{ numElements }, sizeInBytes{ numElements*elementStrideInBytes } {}
				
	};
	
	struct FrameData
	{
		BufferInfo meshTransforms;
		BufferInfo meshTransformIndices;
		BufferInfo lightData;		
		BufferInfo tileToLightIndexMap;
		BufferInfo volumeTileFlags;
		BufferInfo activeTileList;
		BufferInfo activeLightIndicesForTiles;
	};


	
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
		volumeTileGridIsInvalidated{ false },
		currentSceneData{ nullptr }
	{		
		UpdateProjection(surfaceDimensions);
		SubmitCommandsToFitVolumeTileGridToProjection(renderer, *renderer.MakeCommandFactory());		
	}

		PipelineData SceneRenderer::MakeDepthOnlyPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{
			Assets::StaticMeshData::ConfigureVertexLayoutSettings(renderer.GetVertexLayoutSettings());//todo: initial setup should not be done in this function. Also, what about different vertex layouts
			renderer.GetVertexLayoutSettings().SaveSettings();
			renderer.GetRasterizerSettings().SetFrontIsCounterClockwise(false).SaveSettings();
		


			renderer.GetSignatureSettings()
			.AddConstant(1, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 2);					
			auto out{ ConsumeRootSignatureSettings(renderer) };

			assetSystem::AssetPtrTyped<Assets::ShaderAsset> vsInstancing{ shaderProvider.GetAsset("Shaders/InstancedVertex.vs.shdr") };										
			out.pso = MakePso(renderer, out.signature, &vsInstancing, nullptr);
					
			return out;
					
		}

			PipelineData SceneRenderer::ConsumeRootSignatureSettings(RendererFacade &renderer)
			{
				PipelineData out;		
				SerializeTarget rootData;
				renderer.SerializeRootSignature(rootData, nullptr, 0);
				out.signature = { &renderer, renderer.MakeRootSignature(rootData.GetData(), rootData.GetSizeInBytes(), 0) };
				renderer.GetSignatureSettings().RestoreSettingsToSaved();

				return out;
		
			}

			HandleWrapper SceneRenderer::MakePso
			(
				RendererFacade &renderer,
				const ResourceHandle::t_hash signature,
				assetSystem::AssetPtrTyped<Assets::ShaderAsset> *const vs,
				assetSystem::AssetPtrTyped<Assets::ShaderAsset> *const ps
			)
			{
				ShaderList shaders{};
				if(vs)
				{
					(*vs)->Compile(renderer);
					shaders.vs.data = (*vs)->GetCompiledCode();
					shaders.vs.sizeInBytes = (*vs)->GetCompiledCodeSizeInBytes();
				}
				
				if(ps)
				{
					(*ps)->Compile(renderer);
					shaders.ps.data = (*ps)->GetCompiledCode();
					shaders.ps.sizeInBytes = (*ps)->GetCompiledCodeSizeInBytes();
				}
		
				return{ &renderer, renderer.MakePso(shaders, signature) };
		
			}

		PipelineData SceneRenderer::MakeVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{								
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 1);		
			auto out{ ConsumeRootSignatureSettings(renderer) };
		
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csCreateVolumeTileGrid{ shaderProvider.GetAsset("Shaders/CreateVolumeTileGrid.cs.shdr") };		
			csCreateVolumeTileGrid->Compile(renderer);
					
			out.pso = { &renderer, renderer.MakePso(Blob{csCreateVolumeTileGrid->GetCompiledCode(), csCreateVolumeTileGrid->GetCompiledCodeSizeInBytes()}, out.signature) };					
			return out;
		
		}
	
		PipelineData SceneRenderer::MakeMarkActiveVolumeTilesPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{
			auto &settings{ renderer.GetSignatureSettings() };			
			settings
			.AddConstant(1, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 2)
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 2, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 1, 1);								
			auto out{ ConsumeRootSignatureSettings(renderer) };
		
			assetSystem::AssetPtrTyped<Assets::ShaderAsset>
				vsInstancing{ shaderProvider.GetAsset("Shaders/InstancedVertex.vs.shdr") },
				psMarkVolumeTiles{ shaderProvider.GetAsset("Shaders/MarkActiveVolumeTiles.ps.shdr") };
				
			out.pso = MakePso(renderer, out.signature, &vsInstancing, &psMarkVolumeTiles);
			return out;		
		
		}
	
		PipelineData SceneRenderer::MakeBuildActiveVolumeTileListPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{		
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.AddConstant(0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 1)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 1);
			auto out{ ConsumeRootSignatureSettings(renderer) };	
					
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csBuildActiveTileList{ shaderProvider.GetAsset("Shaders/BuildActiveTileList.cs.shdr") };		
			csBuildActiveTileList->Compile(renderer);
		
			out.pso = { &renderer, renderer.MakePso(Blob{csBuildActiveTileList->GetCompiledCode(), csBuildActiveTileList->GetCompiledCodeSizeInBytes()}, out.signature) };					
			return out;
		
		}
	
		PipelineData SceneRenderer::MakeAssignLightsToTilesPipeline(assetSystem::AssetSystem &shaderProvider, RendererFacade &renderer)
		{
			auto &settings{ renderer.GetSignatureSettings() };
			settings
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 0, 1)
			.DeclareTable()
			.AddTableRange(&DescriptorTargets::ConstantBuffer, 1, 1)
			.AddTableRange(&DescriptorTargets::ShaderResource, 0, 3)
			.AddTableRange(&DescriptorTargets::UnorderedAccess, 0, 2);
			auto out{ ConsumeRootSignatureSettings(renderer) };
		
			assetSystem::AssetPtrTyped<Assets::ShaderAsset> csBuildActiveTileList{ shaderProvider.GetAsset("Shaders/AssignLightsToTiles.cs.shdr") };		
			csBuildActiveTileList->Compile(renderer);
		
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
			   	
		void SceneRenderer::SubmitCommandsToFitVolumeTileGridToProjection(RendererFacade &renderer, Commands::CommandFactory &cmds)
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
			createGridCommand->Add(cmds.SetSignatureCompute(pipelineCreateVolumeTileGrid.signature));
			createGridCommand->Add(cmds.SetPipelineState(pipelineCreateVolumeTileGrid.pso));
			createGridCommand->Add(cmds.SetDescriptorBlockViewsAsComputeTable(view, 0));
			createGridCommand->Add(cmds.Dispatch
			(
				std::ceil(gridData.outGridDimensions.x / 4.f),
				std::ceil(gridData.outGridDimensions.y / 4.f),
				std::ceil(gridData.outGridDimensions.z / 4.f)
			));
			createGridCommand->Add(cmds.TransitionUnorderedAccessToShaderResource(gridBoundingBoxBuffer, true));

			renderer.SubmitCommand(std::move(createGridCommand));
			volumeTileGridIsInvalidated = false;
		
		}


	
	SceneRenderer::SceneRenderer(SceneRenderer &&) noexcept = default;

	SceneRenderer &SceneRenderer::operator=(SceneRenderer &&) noexcept = default;
	
	SceneRenderer::~SceneRenderer() = default;



	
	void SceneRenderer::SubmitFrame(UniquePtr<GraphVisitorHarvestMeshes> &graphData)
	{
		if(volumeTileGridIsInvalidated)
		{
			SubmitCommandsToFitVolumeTileGridToProjection(mediator->Renderer(), mediator->CommandFactory());
		}
		
		if(graphData->meshInstanceData.empty())
		{
			return;
		}
		
		UploadGlobalBuffer(globalData.time + .1f, mediator->Renderer());//todo provide proper time

		auto &cmds{ mediator->CommandFactory() };

		currentSceneData = graphData.get();
		FrameData frameData{};
		RenderDepthPrePass(frameData);		
		MarkActiveVolumeTiles(frameData);

		BuildActiveTileList(frameData);
		if(hasNoActiveTiles)
		{
			return;
			
		}
		
		RenderColorPass(frameData);
		
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

		void SceneRenderer::RenderDepthPrePass(FrameData &frameData)
		{
			auto &cmds{ mediator->CommandFactory() };
			
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
			currentOffsetsToMeshTransforms.clear();
			currentDrawCommands = MakeUnique<Commands::CompositeCommand>();

			for(auto &&perMeshData : currentSceneData->meshInstanceData)
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
				currentOffsetsToMeshTransforms.emplace_back(static_cast<uint32_t>(currentTransformData.size()));
				AppendMeshTransformsToTransformData(perMeshData.second.transforms);
				
			}

			//upload shard transforms			
			frameData.meshTransforms = { mediator->Renderer(), currentTransformData };
			
				frameData.meshTransformIndices = { mediator->Renderer(), currentTransformIndexData };
			
				HandleWrapper transformTableContent
				{
					&mediator->Renderer(),
					mediator->Renderer().GetViewFactory()
					.DeclareDescriptorBlock(2, 0)
					.CreateShaderResourceView(frameData.meshTransforms.handle, 0, frameData.meshTransforms.numElements, frameData.meshTransforms.sizeInBytes)
					.CreateShaderResourceView(frameData.meshTransformIndices.handle, 0, currentTransformIndexData.size(), frameData.meshTransformIndices.sizeInBytes)
					.FinalizeDescriptorBlock()
				};
			
			//submit tranform bind
			cmd->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(transformTableContent, 2));
			
			//submit draw commands
			cmd->Add(std::move(currentDrawCommands));
			mediator->Renderer().SubmitCommand(std::move(cmd));
						
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

		void SceneRenderer::MarkActiveVolumeTiles(FrameData &frameData)
		{
			auto &cmds{ mediator->CommandFactory() };
		
			//mark active tiles
			//assign lights to active tiles todo: reduce relevant light set before this step
			auto markTilesCommand{ MakeUnique<Commands::CompositeCommand>() };
			markTilesCommand->Add(cmds.SetPipelineState(pipelineMarkActiveVolumeTiles.pso));
			markTilesCommand->Add(cmds.SetSignatureGraphics(pipelineMarkActiveVolumeTiles.signature));
			markTilesCommand->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(globalDataBufferDescriptor, 1));
			
			currentTransformIndexData.clear();
			currentDrawCommands = MakeUnique<Commands::CompositeCommand>();

			
			uint32_t meshIndex{ 0 };
			for(auto &&perMeshData : currentSceneData->meshInstanceData)
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
						AddMeshShardDrawToCurrentCommands(mesh->GetMeshShardInfo(shardIndex), numInstances, shardPipelineOccurence.second.data, currentOffsetsToMeshTransforms.at(meshIndex));																				
					}				
				}
				++meshIndex;
			}

			frameData.meshTransformIndices = { mediator->Renderer(), currentTransformIndexData };

			frameData.volumeTileFlags = { currentVolumeTileGrid.GetTileCount(), sizeof(uint32_t) };					
			frameData.volumeTileFlags.handle = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, frameData.volumeTileFlags.sizeInBytes) };
					
			HandleWrapper tableContent
			{
				&mediator->Renderer(),
				mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineMarkActiveVolumeTiles.signature, 1, 4, 0)
				.DeclareOrdinal(1).CreateShaderResourceView(frameData.meshTransforms.handle, 0, frameData.meshTransforms.numElements, frameData.meshTransforms.elementStrideInBytes)
				.DeclareOrdinal(2).CreateShaderResourceView(frameData.meshTransformIndices.handle, 0, frameData.meshTransformIndices.numElements, frameData.meshTransformIndices.elementStrideInBytes)
				.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
				.DeclareOrdinal(1).CreateUnorderedAccessView(frameData.volumeTileFlags.handle, 0, frameData.volumeTileFlags.numElements, frameData.volumeTileFlags.elementStrideInBytes)
				.FinalizeDescriptorBlock()			
			};
			markTilesCommand->Add(cmds.SetDescriptorBlockViewsAsGraphicsTable(tableContent, 2));
			markTilesCommand->Add(std::move(currentDrawCommands));		
			mediator->Renderer().SubmitCommand(std::move(markTilesCommand));
		
		}

		void SceneRenderer::BuildActiveTileList(FrameData &frameData)
		{
			auto &cmds{ mediator->CommandFactory() };
		
			//build active tile list
			auto buildTileListCmd{ MakeUnique<Commands::CompositeCommand>() };
			buildTileListCmd->Add(cmds.SetPipelineState(pipelineBuildActiveVolumeTileList.pso));
			buildTileListCmd->Add(cmds.SetSignatureCompute(pipelineBuildActiveVolumeTileList.signature));
			buildTileListCmd->Add(cmds.SetComputeConstants(0, currentVolumeTileGrid.GetTileCount(), 1, 0));
			buildTileListCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(frameData.volumeTileFlags.handle, false));

			frameData.activeTileList = { currentVolumeTileGrid.GetTileCount()+1, sizeof(uint32_t) };
			frameData.activeTileList.handle = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, frameData.activeTileList.sizeInBytes) };
			
			HandleWrapper resourceViews
			{
				&mediator->Renderer(),
				mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineBuildActiveVolumeTileList.signature, 0, 2, 0)
				.DeclareOrdinal(1).CreateShaderResourceView(frameData.volumeTileFlags.handle, 0, frameData.volumeTileFlags.numElements, frameData.volumeTileFlags.elementStrideInBytes)
				.DeclareOrdinal(1).CreateUnorderedAccessView(frameData.activeTileList.handle, 0, frameData.activeTileList.numElements, frameData.activeTileList.elementStrideInBytes)
				.FinalizeDescriptorBlock()
			};

			buildTileListCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(resourceViews, 1));
			buildTileListCmd->Add(cmds.Dispatch(std::ceil(currentVolumeTileGrid.GetTileCount() / 32.f), 1, 1));

			mediator->Renderer().SubmitCommand(std::move(buildTileListCmd));
					
			uint32_t numActiveTiles{ 0 };
			mediator->Renderer().QueryUaResourceContent(frameData.activeTileList.handle, sizeof(numActiveTiles), &numActiveTiles);

			hasNoActiveTiles = numActiveTiles <= 0;
			if(hasNoActiveTiles)
			{
				return;
				
			}
					
			if(currentSceneData->lightData.empty())		
			{
				return;
				
			}
		
			frameData.lightData = { mediator->Renderer(), currentSceneData->lightData };
			
			frameData.activeLightIndicesForTiles = { numActiveTiles*currentSceneData->lightData.size(), sizeof(uint16_t) };
			frameData.activeLightIndicesForTiles.handle = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, frameData.activeLightIndicesForTiles.sizeInBytes) };

			frameData.tileToLightIndexMap = { currentVolumeTileGrid.GetTileCount(), sizeof(LightIndexListInfo) };
			frameData.tileToLightIndexMap.handle = { &mediator->Renderer(), mediator->Renderer().MakeUaBuffer(nullptr, frameData.tileToLightIndexMap.sizeInBytes) };
					
			HandleWrapper assignLightsTable
			{
				&mediator->Renderer(),
				mediator->Renderer().GetViewFactory().DeclareSpecificDescriptorBlock(pipelineAssignLightsToTiles.signature, 1, 6, 0)
				.DeclareOrdinal(1).CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
				.DeclareOrdinal(1).CreateShaderResourceView(frameData.lightData.handle, 0, frameData.lightData.numElements, frameData.lightData.elementStrideInBytes)
				.DeclareOrdinal(2).CreateShaderResourceView(gridBoundingBoxBuffer, 0, currentVolumeTileGrid.GetTileCount(), currentVolumeTileGrid.GetTileStride())
				.DeclareOrdinal(3).CreateShaderResourceView(frameData.activeTileList.handle, 0, frameData.activeTileList.numElements, frameData.activeTileList.elementStrideInBytes)
				.DeclareOrdinal(1).CreateUnorderedAccessView(frameData.activeLightIndicesForTiles.handle, 0, frameData.activeLightIndicesForTiles.numElements, frameData.activeLightIndicesForTiles.elementStrideInBytes)
				.DeclareOrdinal(2).CreateUnorderedAccessView(frameData.tileToLightIndexMap.handle, 0, frameData.tileToLightIndexMap.numElements, frameData.tileToLightIndexMap.elementStrideInBytes)
				.FinalizeDescriptorBlock()
			};

			auto assignLightsCmd{ MakeUnique<Commands::CompositeCommand>() };
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(frameData.activeTileList.handle, false));
			assignLightsCmd->Add(cmds.SetPipelineState(pipelineAssignLightsToTiles.pso));
			assignLightsCmd->Add(cmds.SetSignatureCompute(pipelineAssignLightsToTiles.signature));
			assignLightsCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(globalDataBufferDescriptor, 0));
			assignLightsCmd->Add(cmds.SetDescriptorBlockViewsAsComputeTable(assignLightsTable, 1));
			assignLightsCmd->Add(cmds.Dispatch(std::ceil(numActiveTiles /32.f), 1, 1));
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(frameData.activeLightIndicesForTiles.handle, true));
			assignLightsCmd->Add(cmds.TransitionUnorderedAccessToShaderResource(frameData.tileToLightIndexMap.handle, true));
						
			mediator->Renderer().SubmitCommand(std::move(assignLightsCmd));			
				
		}

		void SceneRenderer::RenderColorPass(FrameData &frameData)
		{
			auto &cmds{ mediator->CommandFactory() };
		
			//render with user pixel
			currentDrawCommands = MakeUnique<Commands::CompositeCommand>();
			currentTransformIndexData.clear();
			uint32_t meshIndex{ 0 };
					
			for(auto &&perMeshData : currentSceneData->meshInstanceData)
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
						AddMeshShardDrawToCurrentCommands(mesh->GetMeshShardInfo(shardIndex), numInstances, shardPipelineOccurence.second.data, currentOffsetsToMeshTransforms.at(meshIndex));																				
					}				
				}
				++meshIndex;
			}

			frameData.meshTransformIndices = { mediator->Renderer(), currentTransformIndexData };
			
			//add context command binding all static resources/
			HandleWrapper lightingPassTable
			{
				&mediator->Renderer(),
				mediator->Renderer().GetViewFactory().DeclareDescriptorBlock(6, 0)
				.CreateConstantBufferView(gridDataBuffer, gridDataBufferSizeInBytes)
				.CreateShaderResourceView(frameData.meshTransforms.handle, 0, frameData.meshTransforms.numElements, frameData.meshTransforms.elementStrideInBytes)
				.CreateShaderResourceView(frameData.meshTransformIndices.handle, 0, frameData.meshTransformIndices.numElements, frameData.meshTransformIndices.elementStrideInBytes)
				.CreateShaderResourceView(frameData.lightData.handle, 0, frameData.lightData.numElements, frameData.lightData.elementStrideInBytes)
				.CreateShaderResourceView(frameData.activeLightIndicesForTiles.handle, 0, frameData.activeLightIndicesForTiles.numElements, frameData.activeLightIndicesForTiles.elementStrideInBytes)
				.CreateShaderResourceView(frameData.tileToLightIndexMap.handle, 0, frameData.tileToLightIndexMap.numElements, frameData.tileToLightIndexMap.elementStrideInBytes)
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
