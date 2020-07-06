#include "RendererFacadeImpl.hpp"
#include "RHA/Interface/DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Resources/HandleFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/ResourceFactoryDeallocatable.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Interface/Resources/SerializeTarget.hpp"
#include "Utility/Alignment.hpp"
#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Commands/UserContextCommandWrapper.hpp"
#include "Commands/DX12CommandFactory.hpp"


#if DEBUG_OPTIMIZED
	constexpr bool enableDebugLayers = true;
	constexpr bool enableGpuValidation = false;
#elif _DEBUG
	constexpr bool enableDebugLayers = true;
	constexpr bool enableGpuValidation = true;
#else
	constexpr bool enableDebugLayers = false;
	constexpr bool enableGpuValidation = false;	
#endif


namespace Renderer::DX12
{
	using namespace RHA::DX12;

	
	RendererFacadeImpl::RendererFacadeImpl(HWND outputWindow)
		:		
		resources{ Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, enableDebugLayers, enableGpuValidation) },
		commonQueue{ Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT) },
		closeFence{ Facade::MakeFence(resources.get()) },
		closeEvent{ CreateEvent(nullptr, false, false, nullptr) },
		resourceFactory
		{
			std::make_unique<ResourceFactoryDeallocatable>
			(
				resources.get(),
				commonQueue.get(),
				std::make_unique<ResourceMemory>
				(
					resources.get(),
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 32,
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
					D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS 
				),
				std::make_unique<ResourceMemory>
				(
					resources.get(),
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 16,
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
					D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES
				)
			)
		},
		psoFactory{ resources.get(), depthStencilSettings, blendSettings, rasterizerSettings, vertexLayoutSettings },
		signatureFactory{ resources.get(), signatureSettings },
		shaderFactory{ Facade::MakeShaderFactory(5, 1) },
		descriptors{ resources.get(), 524'288, 512 },		
		commandProcessor{ *resources, *commonQueue, registry, counterFactory },
		resourceViewFactory{ registry, descriptors }
	{							
		commandProcessor.SubmitContextCommand(std::make_unique<Commands::BindDescriptorsContextCommand>(descriptors));
		
		/*
		VolumeTileGridData gridData;
		gridData.screenDimensions.x = outputSurface->GetWidth();
		gridData.screenDimensions.y = outputSurface->GetHeight();
		gridData.nearDistance = 10;
		gridData.farDistance = 50'000.f;
		const auto fov{ Math::Radians(90.f) };
		
		globalsToDispatch.projection = Math::Matrix::MakeProjection(fov, gridData.screenDimensions.x, gridData.screenDimensions.y, gridData.nearDistance, gridData.farDistance);
		gridData.inverseProjection = globalsToDispatch.projection.Inverse();

		{			
			volumeTileGrid = VolumeTileGrid{ Math::VectorUint2{128,128}, 90.f, gridData };

			SerializeContainer s{};
			SerializeRootSignature(0,0,1,0, &s);
			uav1Signature = HandleWrapper{ this, MakeRootSignature(s.GetData()) };

			std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/ComputeVolumeTileGridBB.cs"), std::ios_base::in | std::ios_base::ate};
			SerializeContainer cs{};
			{					
				const auto csCharCount{ shaderFile.tellg() };
				shaderFile.seekg(0);

				auto csshader{ std::make_unique<char[]>(csCharCount) };
				shaderFile.read( csshader.get(), csCharCount);
							
				CompileComputeShader(csshader.get(), csCharCount, &cs);

				shaderFile.close();
			}

			Blob csBlob{cs.GetData(), cs.GetSize()};
			auto compPso{ MakePso(csBlob, uav1Signature) };			

			auto commandHandle
			{
				commandProcessor.SubmitExtractableCommand( cmdFactory.MakeCommand<Commands::InitVolumeTileGridCommand>
				(
						uav1Signature.Get(),
						compPso,
						std::move(volumeTileGrid),	
						gridData
				))
			};			

			commandProcessor.WaitForCommand(commandHandle);
			initGridCmd.reset( static_cast<Commands::InitVolumeTileGridCommand *>(commandProcessor.ExtractCommand(commandHandle).release()) );
			initGridCmd->WriteTileData(volumeTileGrid);
			initGridCmd->ExecuteOperationOnResourceReferences(registry, &UsesReferences::AddReference);
			
		}



	//_-----------------------
		SerializeContainer root{};
		SerializeRootSignature(0,0,0,0, &root);
		defaultSignature = HandleWrapper{ this, MakeRootSignature(root.GetData()) };

		root.Reset();
		SerializeRootSignature(1,0,1,0, &root);
		markActiveTilesSignature = HandleWrapper{ this, MakeRootSignature(root.GetData()) };

		root.Reset();
		SerializeRootSignature(0, 1, 1, 0, &root);
		buildTileListSignature = HandleWrapper{ this, MakeRootSignature(root.GetData()) };		

		root.Reset();
		SerializeRootSignature(0, 3, 2, 0, &root);
		assignLightsSignature = HandleWrapper{ this, MakeRootSignature(root.GetData()) };
		
		SerializeContainer vs{};
		{
			std::ifstream shaderFile{ Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/PlainMinstance.vs"), std::ios_base::in | std::ios_base::ate };
			
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( pshader.get(), charCount);
										
			CompileVertexShader(pshader.get(), charCount, &vs);
						
		}

		SerializeContainer ps{};
		{
			std::ifstream shaderFile{ Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/MarkActiveVolumeTiles.ps"), std::ios_base::in | std::ios_base::ate };
			
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( pshader.get(), charCount);
										
			CompilePixelShader(pshader.get(), charCount, &ps);
						
		}
		
		{
			ShaderList shaderList{};
			shaderList.vs.data = vs.GetData();
			shaderList.vs.sizeInBytes = vs.GetSize();

			depthOnlyPso = HandleWrapper{ this, MakePso(PipelineTypes::Opaque, VertexLayoutTypes::Position, shaderList, defaultSignature) };
		}

		{
			ShaderList shaderList{};
			shaderList.vs.data = vs.GetData();
			shaderList.vs.sizeInBytes = vs.GetSize();
			
			shaderList.ps.data = ps.GetData();
			shaderList.ps.sizeInBytes = ps.GetSize();
			
			markActiveTilesPso = HandleWrapper{ this, MakePso(PipelineTypes::Opaque, VertexLayoutTypes::Position, shaderList, markActiveTilesSignature) };

			Commands::FlagActiveVolumeTilesCommand::SetPso(markActiveTilesPso);
			Commands::FlagActiveVolumeTilesCommand::SetSignature(markActiveTilesSignature);
			
		}

		//build tile list
		{
			SerializeContainer cs{};
			std::ifstream shaderFile{ Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/BuildActiveTileList.cs"), std::ios_base::in | std::ios_base::ate };
			
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( pshader.get(), charCount);
										
			CompileComputeShader(pshader.get(), charCount, &cs);
			buildTileListPso = HandleWrapper{ this, MakePso({ cs.GetData(), cs.GetSize()}, buildTileListSignature) };

			Commands::BuildActiveTileListCommand::SetPso(buildTileListPso);
			Commands::BuildActiveTileListCommand::SetSignature(buildTileListSignature);
		}			

		//assign lights
		{
			SerializeContainer cs{};
			std::ifstream shaderFile{ Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/AssignLightsToTiles.cs"), std::ios_base::in | std::ios_base::ate };
			
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( pshader.get(), charCount);
										
			CompileComputeShader(pshader.get(), charCount, &cs);
			assignLightsPso = HandleWrapper{ this, MakePso({ cs.GetData(), cs.GetSize()}, assignLightsSignature) };

			Commands::AssignLightsToTilesCommand::SetPso(assignLightsPso);
			Commands::AssignLightsToTilesCommand::SetSignature(assignLightsSignature);
		}			

	//-----------------------
		*/
		
	}
		   

	
	RendererFacadeImpl::~RendererFacadeImpl()
	{			
		WaitForIdleQueue();
		
	}

		void RendererFacadeImpl::WaitForIdleQueue()
		{
			closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
			closeFence->Signal(1, commonQueue.get());
			WaitForSingleObject(closeEvent, INFINITE);
			closeFence->Signal(0);
		
		}


	
		/*
		const auto currentTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.f };
		const auto dispatchDelta{ currentTime - lastDispatchTime };
		lastDispatchTime = currentTime;
		globalsToDispatch.time += dispatchDelta;

		globalBuffer = HandleWrapper{ this, MakeBuffer(&globalsToDispatch, sizeof globalsToDispatch) };
		commandProcessor.SubmitContextCommand(cmdFactory.MakeCommand<Commands::GlobalBufferContextCommand>(descriptors, std::move(globalBuffer)));

		
		//depth pre pass for opaques
		RenderSurface depthOnlySurface
		{
			nullptr,
			depthSurface.get(),
			outputSurface->GetWidth(),
			outputSurface->GetHeight()
		};
		
		commandProcessor.SubmitCommand(cmdFactory.MakeCommand<Commands::PrepareSurfaceForRenderingCommand>(depthOnlySurface));

		for(auto &&args : opaqueMeshArguments)
		{
			commandProcessor.SubmitCommand(cmdFactory.MakeCommand<Commands::RenderMeshCommand>(defaultSignature.Get(), depthOnlyPso.Get(), args));			
		}


		//make light buffer
		lightsBuffer = HandleWrapper{ this, MakeBuffer(registry.GetLightsData(), registry.GetLigthsDataSizeInBytes(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)};

		
		//make active tile list
		auto activeTilesCmd
		{
			cmdFactory.MakeCommand<Commands::GenerateActiveTileListCommand>
			(
				registry.GetLightCount(),
				volumeTileGrid.GetTileCount(),
				initGridCmd->GetGridAABBBufferHandle(),
				lightsBuffer.Get(),
				initGridCmd->GetGridDataBufferHandle()
			)
		};

		for(auto &&args : opaqueMeshArguments)
		{
			activeTilesCmd->AddRenderMeshCommand(args);			
		}
				
		auto generateTileListHandle{ commandProcessor.SubmitExtractableCommand(std::move(activeTilesCmd)) };
		
		commandProcessor.WaitForCommand(generateTileListHandle);
		activeTilesCmd = UniquePtr<Commands::GenerateActiveTileListCommand>(static_cast<Commands::GenerateActiveTileListCommand *>(commandProcessor.ExtractCommand(generateTileListHandle).release()));

		
		//assign lights to tiles
		auto assignLightCmd{ cmdFactory.MakeCommand<Commands::AssignLightsToTilesCommand>
		(				
			activeTilesCmd->activeTileList,
			lightsBuffer.Get(),
			initGridCmd->GetGridAABBBufferHandle(),
			initGridCmd->GetGridDataBufferHandle(),
			registry.GetLightCount(),
			volumeTileGrid.GetTileCount(),
			activeTilesCmd->activeTileCount
		)};
		auto relevantLightsList{ assignLightCmd->GetRelevantLightIndexList() };
		auto mappingToRelevantLights{ assignLightCmd->GetMappingToRelevantLights() };		
		commandProcessor.SubmitCommand(std::move(assignLightCmd));


		//context change
		globalBuffer = HandleWrapper{ this, MakeBuffer(&globalsToDispatch, sizeof globalsToDispatch) };
		commandProcessor.SubmitContextCommand(std::make_unique<Commands::LightingContextCommand>
		(
			descriptors, 											  
			std::move(globalBuffer),
			opaqueMeshArguments.at(0).signature,//todo: remove hack
			initGridCmd->GetGridDataBufferHandle(),
			lightsBuffer,
			relevantLightsList,
			mappingToRelevantLights,
			volumeTileGrid.GetTileCount(),
			activeTilesCmd->activeTileCount,
			registry 
		));


		//render with lighting
		RenderSurface renderSurface
		{
			outputSurface.get(),
			depthSurface.get(),
			outputSurface->GetWidth(),
			outputSurface->GetHeight()
		};		
		renderSurface.ShouldClearDepthSurface(false);
		commandProcessor.SubmitCommand(cmdFactory.MakeCommand<Commands::PrepareSurfaceForRenderingCommand>(renderSurface));

		for(auto &&meshArgs : opaqueMeshArguments)
		{
			commandProcessor.SubmitCommand(std::make_unique<Commands::RenderMeshCommand>(meshArgs));			
		}
		opaqueMeshArguments.clear();

		//schedule presentation todo: take care of sync
		commandProcessor.SubmitCommand(cmdFactory.MakeCommand<Commands::PrepareSurfaceForPresentCommand>(renderSurface));
		commandProcessor.SubmitCommand(cmdFactory.MakeCommand<Commands::PresentSurfaceCommandOld>(renderSurface));


		registry.PurgeUnreferencedEntities();
		commandProcessor.FreeExecutedCommands();
		*/

	
	size_t RendererFacadeImpl::MakeBuffer(const void *data, const size_t sizeInBytes)
	{			
		return registry.Register
		(
			resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)				
		);
		
	}

		size_t RendererFacadeImpl::MakeBufferInternal(const void *data, const size_t sizeInBytes, const size_t handle)
		{
			auto allocation
			{
				resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
			};

		
			if(handle == 0)
			{
				return registry.Register(std::move(allocation));
				
			}

			registry.Register(handle, std::move(allocation));
			return handle;
		
		}

	
	
	size_t RendererFacadeImpl::MakeBuffer(const void *data, const size_t sizeInBytes, const D3D12_RESOURCE_STATES state)
	{
		return registry.Register
		(					
			resourceFactory->MakeBufferWithData(data, sizeInBytes, state)						
		);	
	}



	size_t RendererFacadeImpl::MakeUavBuffer(const void *data, const size_t sizeInBytes)
	{						
		return registry.Register
		(					
			resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)						
		);				
		
	}


	
	DxPtr<ID3D12Resource> RendererFacadeImpl::MakeReadbackBuffer(const size_t sizeInBytes)
	{			
		return resourceFactory->MakeCommittedBuffer(RHA::Utility::IncreaseValueToAlignment(sizeInBytes, 256), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_HEAP_TYPE_READBACK, D3D12_HEAP_FLAG_NONE);
		
	}


	
	void RendererFacadeImpl::AddShaderIncludeDirectory(const char *absoluteDirectoryPath)
	{
		shaderFactory->AddIncludeDirectory(absoluteDirectoryPath);
		
	}



	bool  RendererFacadeImpl::CompileVertexShader(const char *shader, size_t length, SerializationHook &serializer) const
	{
		auto shaderBlob{ shaderFactory->MakeVertexShader(shader, length, "main")};
						
		serializer.Resize(shaderBlob->GetBufferSize());
		std::memcpy(serializer.GetData(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());				
		
	}


	
	bool  RendererFacadeImpl::CompilePixelShader(const char *shader, size_t length, SerializationHook &serializer) const
	{
		auto shaderBlob{ shaderFactory->MakePixelShader(shader, length, "main")};

		serializer.Resize(shaderBlob->GetBufferSize());
		std::memcpy(serializer.GetData(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());	
		
	}



	
	bool RendererFacadeImpl::CompileComputeShader(const char *shader, const size_t length, SerializationHook &serializer) const
	{
		auto shaderBlob{ shaderFactory->MakeComputeShader(shader, length, "main") };

		serializer.Resize(shaderBlob->GetBufferSize());
		std::memcpy(serializer.GetData(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());	
		
	}



	void RendererFacadeImpl::SerializeRootSignature
	(
		SerializationHook &serializer,
		const SamplerSpec *staticSamplers,
		const unsigned numStaticSamplers
	)
	{
		auto signatureBlob{ signatureFactory.SerializeRootSignature(staticSamplers, numStaticSamplers) };
		
		serializer.Resize(signatureBlob->GetBufferSize());
		std::memcpy(serializer.GetData(), signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize());
				
	}


	
	size_t RendererFacadeImpl::MakeRootSignature(const void *serializedData, const size_t dataSizeInBytes, const unsigned samplerAmount)
	{		
		auto signatureData
		{
			signatureFactory.MakeRootSignature
			(
				serializedData,
				dataSizeInBytes,
				samplerAmount
			)
		};
					
		return registry.Register(std::move(signatureData));
					
	}

	

	
	size_t RendererFacadeImpl::MakePso(const ShaderList &shaders, ResourceHandle::t_hash signatureHandle)
	{
		auto pipelineState{	psoFactory.MakePso(shaders, registry.GetSignature(signatureHandle), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) };		
		return registry.Register(std::move(pipelineState));
		
	}


	
	size_t RendererFacadeImpl::MakePso(const Blob &csBlob, Renderer::ResourceHandle::t_hash signatureHandle)
	{
		auto pipelineState{ psoFactory.MakePso(csBlob, registry.GetSignature(signatureHandle)) };
		return registry.Register(std::move(pipelineState));
		
	}


	
	ResourceHandle::t_hash RendererFacadeImpl::MakeTexture(const void *data, const size_t width, const size_t height)
	{
		auto resource{ resourceFactory->MakeTextureWithData(data, width, height, D3D12_RESOURCE_STATE_COMMON) };
		return registry.Register(std::move(resource));
		
	}



	bool RendererFacadeImpl::IsResourceValid(ResourceHandle::t_hash handle)
	{
		return registry.IsHandleUnknown(handle);
		
	}


	
	void RendererFacadeImpl::RetireHandle(ResourceHandle::t_hash handle)
	{		
		registry.RetireHandle(handle);
		
	}


		
	ResourceHandle::t_hash RendererFacadeImpl::MakeWindowsWindowSurface(HWND windowHandle)
	{		
		return registry.Register(RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), windowHandle));
		
	}


	
	void RendererFacadeImpl::SetWindowSurfaceToFullscreen(ResourceHandle::t_hash surface)
	{
		if(registry.IsWindowSurfaceReferenced(surface))
		{
			WaitForCommandsAndQueue();			
		}

		registry.GetSurface(surface)->GoFullscreen();
		
	}

		void RendererFacadeImpl::WaitForCommandsAndQueue()
		{
			commandProcessor.WaitForIdle();
			commandProcessor.SubmitAndWaitForGpuWork();
		
		}



	void RendererFacadeImpl::SetWindowSurfaceToWindowed(ResourceHandle::t_hash surface)
	{
		if(registry.IsWindowSurfaceReferenced(surface))
		{
			WaitForCommandsAndQueue();			
		}

		registry.GetSurface(surface)->GoWindowed();
		
	}


	
	void RendererFacadeImpl::FitWindowSurfaceToWindow(ResourceHandle::t_hash surface)
	{
		if(registry.IsWindowSurfaceReferenced(surface))
		{
			WaitForCommandsAndQueue();			
		}

		registry.GetSurface(surface)->ResizeToWindow();
		
	}



	UniquePtr<::Renderer::Commands::CommandFactory> RendererFacadeImpl::MakeCommandFactory()
	{
		return MakeUnique<Commands::DX12CommandFactory>(registry);
		
	}


	
	void RendererFacadeImpl::SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command)
	{
		commandProcessor.SubmitCommand(std::move(command));
		
	}


	
	void RendererFacadeImpl::SubmitContextCommand(UniquePtr<::Renderer::Commands::Command> &&command)
	{
		commandProcessor.SubmitContextCommand(MakeUnique<Commands::UserContextCommandWrapper>(descriptors, std::move(command)));
		
	}



	void RendererFacadeImpl::DestroyUnreferencedResources()
	{
		registry.PurgeUnreferencedEntities();
		
		
	}


	
	void RendererFacadeImpl::DestroyExecutedCommands()
	{
		commandProcessor.FreeExecutedCommands();
		
	}


	
	BlendSettings &RendererFacadeImpl::GetBlendSettings()
	{
		return blendSettings;
		
	}


	
	DepthStencilSettings &RendererFacadeImpl::GetDepthStencilSettings()
	{		
		return depthStencilSettings;
		
	}


	
	RasterizerSettings &RendererFacadeImpl::GetRasterizerSettings()
	{
		return rasterizerSettings;
		
	}


	
	VertexLayoutSettings &RendererFacadeImpl::GetVertexLayoutSettings()
	{
		return vertexLayoutSettings;
		
	}


	
	RootSignatureSettings &RendererFacadeImpl::GetSignatureSettings()
	{
		return signatureSettings;		
		
	}



	ResourceViewFactory &RendererFacadeImpl::GetViewFactory()
	{
		return resourceViewFactory;
		
	}


	
	CounterFactory &RendererFacadeImpl::GetCounterFactory()
	{
		return counterFactory;
		
	}
	
}
