#include "ForwardRenderer.hpp"
#include "RHA/Interface/DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"

#include "FrameWorker.hpp"
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
#include "Commands/CommandInitVolumeTileGrid.hpp"


#if _DEBUG
	constexpr bool enableDebugLayers = true;
#else
	constexpr bool enableDebugLayers = false;
#endif

#include <chrono>

namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		

		
		ForwardRenderer::ForwardRenderer(HWND outputWindow)
			:
			lastDispatchTime{ 0 },
			maxScheduledFrames{ 2 },
			resources{ Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, enableDebugLayers) },
			commonQueue{ Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT) },
			outputSurface{ Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow) },
			depthSurface{ Facade::MakeDepthSurface(resources.get(), outputSurface->GetResourceTemplate()->GetDesc()) },
			closeFence{ Facade::MakeFence(resources.get()) },
			closeEvent{ CreateEvent(nullptr, false, false, nullptr) },
			psoFactory{ resources.get() },
			signatureFactory{ resources.get() },
			shaderFactory{ Facade::MakeShaderFactory(5, 0) },
			renderThread{ framesToDestruct },
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
					)
				)
			},
			descriptors{resources.get(), 1'000'000, 2048}
		{			
			outputSurface->EnableVerticalSync();
			shaderFactory->AddIncludeDirectory(Filesystem::Conversions::MakeExeRelative("../Content/Shaders/Includes").c_str());


			VolumeTileGridData gridData;
			gridData.screenDimensions.x = outputSurface->GetWidth();
			gridData.screenDimensions.y = outputSurface->GetHeight();
			gridData.nearDistance = 10;
			gridData.farDistance = 50'000.f;
			const auto fov{ Math::Radians(90.f) };
			
			globalsToDispatch.projection = Math::Matrix::MakeProjection(fov, gridData.screenDimensions.x, gridData.screenDimensions.y, gridData.nearDistance, gridData.farDistance);
			gridData.inverseProjection = globalsToDispatch.projection.Inverse();

			{
				
				VolumeTileGrid gbb(Math::VectorUint2{128,128}, 90.f, gridData);

				SerializeContainer s{};
				SerializeRootSignature(0,0,1,0, &s);
				auto rootHandle{ MakeRootSignature(s.GetData()) };

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
				auto compPso{ MakePso(csBlob, rootHandle) };			
				
				commandsToDispatch.push_back( std::make_unique<CommandInitVolumeTileGrid>(rootHandle, compPso, *this, registry, descriptors, std::move(gbb), gridData) );

				renderThread.ScheduleFrameWorker( MakeFrameWorkerFromCommands(false, false, false) );
				renderThread.WaitForIdle();
				auto worker{ framesToDestruct.Pop() };
				static_cast<CommandInitVolumeTileGrid *>(worker.IndexCommand(0).get())->WriteTileData(gbb);
				
			}
			
			
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
			
			}


		
		bool ForwardRenderer::IsBusy() const
		{
			return renderThread.GetScheduledWorkerCount() >= maxScheduledFrames;
			
		}

		

		void ForwardRenderer::DispatchFrame()
		{			
			if(IsBusy())
			{
				AbortDispatch();
				return;
			}
			
			const auto currentTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.f };
			const auto dispatchDelta{ currentTime - lastDispatchTime };
			lastDispatchTime = currentTime;
			globalsToDispatch.time += dispatchDelta;
					   			
			renderThread.ScheduleFrameWorker( MakeFrameWorkerFromCommands(true, true, true) );

			for(; framesToDestruct.Size() > 0; )
			{
				auto frame{ framesToDestruct.Pop() };
				frame.UnregisterResources();				
			}
			
			registry.PurgeUnreferencedEntities();
			
		}

			void ForwardRenderer::AbortDispatch()
			{
				commandsToDispatch.clear();
			
			}

			FrameWorker ForwardRenderer::MakeFrameWorkerFromCommands(bool shouldUseColorSurface, bool shouldUseDepthSurface, bool shouldPresentSurface)
			{					
				HandleWrapper globalBuffer{ this, MakeBuffer(&globalsToDispatch, sizeof globalsToDispatch) };

				const RenderSurface renderSurface
				{
					shouldUseColorSurface ? outputSurface.get() : nullptr,
					shouldUseDepthSurface ? depthSurface.get() : nullptr
				};
			
				FrameWorker worker{ resources.get(), commonQueue.get(), descriptors, registry, renderSurface, std::move(globalBuffer), shouldPresentSurface };			
				for(auto &&cmd : commandsToDispatch)
				{
					worker.AddCommand(std::move(cmd));
				}
				commandsToDispatch.clear();

				return worker;
			
			}

		

		void ForwardRenderer::RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle, size_t instanceCount)
		{
			commandsToDispatch.emplace_back
			(
				std::make_unique<RenderMeshCommand>(signatureHandle, psoHandle, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices, transformBufferHandle, instanceCount)
			);			

		}


		
		void ForwardRenderer::SetCamera(float x, float y, float z, float pitch, float yaw, float roll)
		{
			globalsToDispatch.view = Math::Matrix::MakeRotation(-pitch, -yaw, -roll);
			globalsToDispatch.view.Translate(-x, -y, -z);
			
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


		
		size_t ForwardRenderer::MakeUavBuffer(const void *data, const size_t sizeInBytes)
		{						
			return registry.Register
			(					
				bufferFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)						
			);				
			
		}


		
		DxPtr<ID3D12Resource> ForwardRenderer::MakeReadbackBuffer(const size_t sizeInBytes)
		{			
			return bufferFactory->MakeCommittedBuffer(sizeInBytes, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_HEAP_TYPE_READBACK, D3D12_HEAP_FLAG_NONE);
			
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
			SerializationHook *serializer
		)
		{
			auto signatureBlob{ signatureFactory.SerializeRootSignature(cbvAmount, srvAmount, uavAmount, samplerAmount) };
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


		
		size_t ForwardRenderer::MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle)
		{
			auto pipelineState{	psoFactory.MakePso(shaders, registry.GetSignature(signatureHandle), pipelineType, vertexLayoutProvider.GetLayoutDesc(vertexLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) };		
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

		
	}

	
}