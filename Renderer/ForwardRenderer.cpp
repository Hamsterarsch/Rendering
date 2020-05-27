#include "ForwardRenderer.hpp"
#include "RHA/Interface/DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"

#include "Commands/RenderMeshCommand.hpp"


#include "Math/Interface/Utility.hpp"
#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "ShaderRelevantTypes/VolumeTileGridData.hpp"

#include "Resources/HandleFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/ResourceFactoryDeallocatable.hpp"





#include "Resources/Descriptor/DescriptorMemory.hpp"
#include <fstream>

#include "Interface/Resources/SerializationContainer.hpp"
#include "Commands/InitVolumeTileGridCommand.hpp"

#include "Interface/Resources/SerializationContainer.hpp"

#include "Utility/Alignment.hpp"
#include "Commands/ClearDepthSurfaceCommand.hpp"
#include "Commands/PrepareSurfaceForRenderingCommand.hpp"
#include "Commands/PrepareSurfaceForPresentCommand.h"

#include "ShaderRelevantTypes/Light.hpp"
#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Commands/GlobalBufferContextCommand.hpp"
#include "Commands/LightingSetup/FlagActiveVolumeTilesCommand.hpp"
#include "Commands/LightingSetup/BuildActiveTileListCommand.hpp"
#include "Commands/LightingSetup/AssignLightsToTilesCommand.hpp"
#include "Commands/LightingContextCommand.hpp"
#include "Commands/PresentSurfaceCommandOld.hpp"
#include "Commands/LightingSetup/GenerateActiveTileListCommand.hpp"

#include <chrono>
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

	
	ForwardRenderer::ForwardRenderer(HWND outputWindow)
		:
		lastDispatchTime{ 0 },
		maxScheduledFrames{ 2 },
		resources{ Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, enableDebugLayers, enableGpuValidation) },
		commonQueue{ Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT) },
		closeFence{ Facade::MakeFence(resources.get()) },
		closeEvent{ CreateEvent(nullptr, false, false, nullptr) },
		bufferFactory
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
		signatureFactory{ resources.get() },
		shaderFactory{ Facade::MakeShaderFactory(5, 1) },
		descriptors{resources.get(), 1'000'000, 2048},
		cmdFactory{ *this, registry, descriptors },
		commandProcessor{ *resources, *commonQueue, registry }
	{			
		shaderFactory->AddIncludeDirectory(Filesystem::Conversions::MakeExeRelative("../Content/Shaders/Includes").c_str());
						
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
		   

	
	ForwardRenderer::~ForwardRenderer()
	{			
		WaitForIdleQueue();
		
	}

		void ForwardRenderer::WaitForIdleQueue()
		{
			closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
			closeFence->Signal(1, commonQueue.get());
			WaitForSingleObject(closeEvent, INFINITE);
			closeFence->Signal(0);
		
		}


	
	bool ForwardRenderer::IsBusy() const
	{
		return false;
		
	}


	
	void ForwardRenderer::DispatchFrame()
	{		
		if(IsBusy())
		{
			AbortDispatch();
			return;
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
	}

		void ForwardRenderer::AbortDispatch()
		{		
			opaqueMeshArguments.clear();
		
		}
	

	void ForwardRenderer::RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle, size_t instanceCount)
	{
		//todo branch between translucent/opaque based on pso class
		opaqueMeshArguments.emplace_back
		(
			Commands::RenderMeshArguments{ signatureHandle, psoHandle,  transformBufferHandle, instanceCount, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices }
		);			

	}


	
	void ForwardRenderer::SetCamera(float x, float y, float z, float pitch, float yaw, float roll)
	{
		globalsToDispatch.view = Math::Matrix::MakeRotation(-pitch, -yaw, -roll);
		globalsToDispatch.view.Translate(-x, -y, -z);

		globalsToDispatch.inverseView = globalsToDispatch.view.Inverse();		
		globalsToDispatch.inverseProjection = globalsToDispatch.projection.Inverse();

	}


	
	size_t ForwardRenderer::MakeLight(const float (& position)[3], const float (& rotation)[3], const float(& color)[3], float radius)
	{
		Light light{};
		
		light.worldPos.x = position[0];
		light.worldPos.y = position[1];
		light.worldPos.z = position[2];

		auto v = Math::Matrix::MakeRotation(rotation[0], rotation[1], rotation[2]).Transform({0,0,1,1});
		light.worldForwardVector.x = v.x;
		light.worldForwardVector.y = v.y;
		light.worldForwardVector.z = v.z;
		
		light.color.x = color[0];
		light.color.x = color[1];
		light.color.x = color[2];
				
		light.radius = radius;

		return registry.Register(std::move(light));
				
	}

		
	size_t ForwardRenderer::MakeBuffer(const void *data, const size_t sizeInBytes)
	{			
		return registry.Register
		(
			bufferFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)				
		);
		
	}

		size_t ForwardRenderer::MakeBufferInternal(const void *data, const size_t sizeInBytes, const size_t handle)
		{
			auto allocation
			{
				bufferFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
			};

		
			if(handle == 0)
			{
				return registry.Register(std::move(allocation));
				
			}

			registry.Register(handle, std::move(allocation));
			return handle;
		
		}
		

	
	void ForwardRenderer::RemakeBuffer(const void* data, size_t sizeInBytes, size_t handle)
	{
		MakeBufferInternal(data, sizeInBytes, handle);
		
	}


	
	size_t ForwardRenderer::MakeBuffer(const void *data, const size_t sizeInBytes, const D3D12_RESOURCE_STATES state)
	{
		return registry.Register
		(					
			bufferFactory->MakeBufferWithData(data, sizeInBytes, state)						
		);	
	}



	size_t ForwardRenderer::MakeUavBuffer(const void *data, const size_t sizeInBytes)
	{						
		return registry.Register
		(					
			bufferFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)						
		);				
		
	}


	
	DxPtr<ID3D12Resource> ForwardRenderer::MakeReadbackBuffer(const size_t sizeInBytes)
	{			
		return bufferFactory->MakeCommittedBuffer(RHA::Utility::IncreaseValueToAlignment(sizeInBytes, 256), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_HEAP_TYPE_READBACK, D3D12_HEAP_FLAG_NONE);
		
	}

	

	void ForwardRenderer::CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const
	{
		auto shaderBlob{ shaderFactory->MakeVertexShader(shader, length, "main")};

		auto block{ serializer->BeginBlock(shaderBlob->GetBufferSize()) };
		serializer->WriteToBlock(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());			
		
	}


	
	void ForwardRenderer::CompilePixelShader(const char *shader, size_t length, SerializationHook* serializer) const
	{
		auto shaderBlob{ shaderFactory->MakePixelShader(shader, length, "main")};

		auto block{ serializer->BeginBlock(shaderBlob->GetBufferSize()) };
		serializer->WriteToBlock(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
		
	}



	
	void ForwardRenderer::CompileComputeShader(const char *shader, const size_t length, SerializationHook *serializer) const
	{
		auto shaderBlob{ shaderFactory->MakeComputeShader(shader, length, "main") };

		auto block{ serializer->BeginBlock(shaderBlob->GetBufferSize() )};
		serializer->WriteToBlock(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
		
	}

	

	void ForwardRenderer::SerializeRootSignature
	(
		unsigned cbvAmount,
		unsigned srvAmount, 
		unsigned uavAmount,
		unsigned samplerAmount, 
		SerializationHook *serializer,
		const SamplerSpec *staticSamplers,
		const unsigned numStaticSamplers
	)
	{
		auto signatureBlob{ signatureFactory.SerializeRootSignature(cbvAmount, srvAmount, uavAmount, samplerAmount, staticSamplers, numStaticSamplers) };
		const auto signatureSize{ signatureBlob->GetBufferSize() };

		auto block{ serializer->BeginBlock(sizeof signatureSize + signatureBlob->GetBufferSize() + sizeof samplerAmount) };
		serializer->WriteToBlock(&signatureSize, sizeof signatureSize);
		serializer->WriteToBlock(signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize());
		
		serializer->WriteToBlock(&samplerAmount, sizeof samplerAmount);
		
	}


	
	size_t ForwardRenderer::MakeRootSignature(const void *serializedData)
	{
		const auto size{ ExtractSizeFrom(serializedData) };
		auto signatureData
		{
			signatureFactory.MakeRootSignature
			(
				ExtractSignatureFrom(serializedData), 
				size,
				ExtractSamplerCountFrom(serializedData, size)
			)
		};
					
		return registry.Register(std::move(signatureData));
					
	}

		SIZE_T ForwardRenderer::ExtractSizeFrom(const void *data)
		{
			return *reinterpret_cast<const SIZE_T *>(data);
		
		}

		const unsigned char *ForwardRenderer::ExtractSignatureFrom(const void *data)
		{
			return reinterpret_cast<const unsigned char *>(data) + sizeof SIZE_T;
		
		}

		size_t ForwardRenderer::ExtractSamplerCountFrom(const void *data, const SIZE_T signatureSize)
		{
			return *reinterpret_cast<const size_t *>
			(
				reinterpret_cast<const unsigned char *>(data)
				+ sizeof SIZE_T
				+ signatureSize
			);
		
		}


	
	size_t ForwardRenderer::MakePso(const ShaderList &shaders, size_t signatureHandle)
	{
		auto pipelineState{	psoFactory.MakePso(shaders, registry.GetSignature(signatureHandle), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) };		
		return registry.Register(std::move(pipelineState));
		
	}


	
	size_t ForwardRenderer::MakePso(const Blob &csBlob, const size_t signatureHandle)
	{
		auto pipelineState{ psoFactory.MakePso(csBlob, registry.GetSignature(signatureHandle)) };
		return registry.Register(std::move(pipelineState));
		
	}


	
	bool ForwardRenderer::ResourceMustBeRemade(size_t handle)
	{
		return registry.IsHandleUnknown(handle);
		
	}


	
	void ForwardRenderer::RetireHandle(const size_t handle)
	{		
		registry.RetireHandle(handle);
		
	}


		
	ResourceHandle::t_hash ForwardRenderer::MakeWindowsWindowSurface(HWND windowHandle)
	{		
		return registry.Register(RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), windowHandle));
		
	}


	
	UniquePtr<::Renderer::Commands::CommandFactory> ForwardRenderer::MakeCommandFactory()
	{
		return MakeUnique<Commands::DX12CommandFactory>(registry);
		
	}


	
	void ForwardRenderer::SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command)
	{
		commandProcessor.SubmitCommand(UniquePtr<Commands::DX12Command>{static_cast<Commands::DX12Command *>(command.release())});
		
	}



	void ForwardRenderer::DestroyUnreferencedResources()
	{
		registry.PurgeUnreferencedEntities();
		
		
	}


	
	void ForwardRenderer::DestroyExecutedCommands()
	{
		commandProcessor.FreeExecutedCommands();
		
	}


	
	void ForwardRenderer::WaitForCommands()
	{
		commandProcessor.WaitForIdle();
		
	}


	
	BlendSettings &ForwardRenderer::GetBlendSettings()
	{
		return blendSettings;
		
	}


	
	DepthStencilSettings &ForwardRenderer::GetDepthStencilSettings()
	{		
		return depthStencilSettings;
		
	}


	
	RasterizerSettings &ForwardRenderer::GetRasterizerSettings()
	{
		return rasterizerSettings;
		
	}


	
	VertexLayoutSettings &ForwardRenderer::GetVertexLayoutSettings()
	{
		return vertexLayoutSettings;
		
	}
	
	
}
