#include "Renderer.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"


#include "Resources/ResourceAllocation.hpp"
#include "Resources/HandleFactory.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"

#include "FrameRenderer.hpp"
#include "Commands/RenderMeshCommand.hpp"
#include "Resources/Pso/VertexLayoutProvider.hpp"
#include "Shared/Types/Containers/QueueConcurrent.hpp"
#include "Resources/GlobalBufferData.hpp"
#include "Resources/ResourceFactoryDeallocatable.hpp"
#include "RendererMaster.hpp"


#if _DEBUG
	constexpr bool enableDebugLayers = true;
#else
	constexpr bool enableDebugLayers = false;
#endif

#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include <chrono>
	
namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		struct Renderer::PrivateMembers
		{
			HandleFactory handleFactory;
			
			ResourceRegistry registry;
			
			VertexLayoutProvider vertexLayoutProvider;
			
			PsoFactory psoFactory;
			
			RootSignatureFactory signatureFactory;
			
			UniquePtr<ShaderFactory> shaderFactory;
			
			QueueConcurrent<FrameRenderer> framesToDestruct;

			std::future<int> activeFrameHandle;
						
			std::list<UniquePtr<RenderCommand>> commandsToDispatch;

			GlobalBufferData globalsToDispatch;

			std::forward_list<size_t> handlesToRetire;

			RendererMaster renderThread;
			
			PrivateMembers(DeviceResources *resources, unsigned char maxScheduledFrames) :
				psoFactory{ resources },
				signatureFactory{ resources },
				shaderFactory{ Facade::MakeShaderFactory(5, 0) },
				renderThread{ framesToDestruct, maxScheduledFrames }
			{				
			}				
			
		};


		
		Renderer::Renderer(HWND outputWindow) :
			privateMembers{ nullptr },
			lastDispatchTime{ 0 },
			maxScheduledFrames{ 2 }
		{
			resources = Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, enableDebugLayers);
			commonQueue = Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);			

			outputSurface = Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
			outputSurface->EnableVerticalSync();			
			depthSurface = Facade::MakeDepthSurface(resources.get(), outputSurface->GetResourceTemplate()->GetDesc());
			
			closeFence = Facade::MakeFence(resources.get());
			closeEvent = CreateEvent(nullptr, false, false, nullptr);
						
			resourceFactory = std::make_unique<ResourceFactoryDeallocatable>
			(
				resources.get(),
				commonQueue.get(),
				std::make_unique<ResourceMemory>(resources.get(), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 15, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS)
			);			

			privateMembers = std::make_unique<PrivateMembers>(resources.get(), maxScheduledFrames);					   			
						
			privateMembers->globalsToDispatch.projection = glm::perspectiveFovLH_ZO(glm::radians(90.f), outputSurface->GetWidth(), outputSurface->GetHeight(), .01f, 1000.f);
						
		}
			   
	
		
		Renderer::~Renderer()
		{			
			WaitForIdleQueue();
			
		}

			void Renderer::WaitForIdleQueue()
			{
				closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
				closeFence->Signal(1, commonQueue.get());
				WaitForSingleObject(closeEvent, INFINITE);
			
			}

		

		void Renderer::DispatchFrame()
		{			
			if(privateMembers->renderThread.HasNoCapacityForFrames())
			{
				AbortDispatch();
				return;
			}
			
			const auto currentTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.f };
			const auto dispatchDelta{ currentTime - lastDispatchTime };
			lastDispatchTime = currentTime;
			privateMembers->globalsToDispatch.time += dispatchDelta;
					   			
			privateMembers->renderThread.ScheduleFrame( MakeFrameFromCommands() );

			for(; privateMembers->framesToDestruct.Size() > 0; )
			{
				auto frame{ privateMembers->framesToDestruct.Pop() };
				frame.UnregisterResources();				
			}
			
			privateMembers->registry.PurgeUnreferencedResources();
			{
				privateMembers->handlesToRetire.remove_if([ &p = privateMembers ](const size_t &handle)
				{
					if(p->registry.IsHandleUnknown(handle))
					{
						p->handleFactory.RetireHandle(ResourceHandle{ handle });
						return true;					
					}
					return false;
					
				});
			}
			
		}

			void Renderer::AbortDispatch()
			{
				privateMembers->commandsToDispatch.clear();
			
			}

			FrameRenderer Renderer::MakeFrameFromCommands()
			{					
				HandleWrapper globalBuffer{ this, MakeBuffer(&privateMembers->globalsToDispatch, sizeof decltype(privateMembers)::element_type::globalsToDispatch) };
									
				FrameRenderer renderer{ resources.get(), commonQueue.get(), privateMembers->registry, *outputSurface, *depthSurface, std::move(globalBuffer) };			
				for(auto &&cmd : privateMembers->commandsToDispatch)
				{
					renderer.AddCommand(std::move(cmd));
				}
				privateMembers->commandsToDispatch.clear();

				return renderer;
			
			}

		

		void Renderer::RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle, size_t instanceCount)
		{
			privateMembers->commandsToDispatch.emplace_back
			(
				std::make_unique<RenderMeshCommand>(signatureHandle, psoHandle, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices, transformBufferHandle, instanceCount)
			);			

		}


		
		void Renderer::SetCamera(float x, float y, float z, float pitch, float yaw, float roll)
		{
			privateMembers->globalsToDispatch.view = glm::identity<glm::mat4>();
			privateMembers->globalsToDispatch.view = rotate	  (privateMembers->globalsToDispatch.view, glm::radians(-pitch), {1,0,0});
			privateMembers->globalsToDispatch.view = rotate	  (privateMembers->globalsToDispatch.view, glm::radians(-yaw), {0,1,0});
			privateMembers->globalsToDispatch.view = rotate	  (privateMembers->globalsToDispatch.view, glm::radians(-roll), {0,0,1});
			privateMembers->globalsToDispatch.view = translate(privateMembers->globalsToDispatch.view, {-x, -y, -z});
			
		}

		

		size_t Renderer::MakeBuffer(const void *data, const size_t sizeInBytes)
		{
			auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Buffer) };

			return MakeBufferInternal(data, sizeInBytes, handle.hash);
			
		}

			size_t Renderer::MakeBufferInternal(const void *data, const size_t sizeInBytes, const size_t handle)
			{
				auto allocation
				{
					resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
				};

				privateMembers->registry.RegisterResource(handle, std::move(allocation));

				return handle;
			
			}
			

		
		void Renderer::RemakeBuffer(const void* data, size_t sizeInBytes, size_t handle)
		{
			MakeBufferInternal(data, sizeInBytes, handle);
			
		}

		

		void Renderer::CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const
		{
			auto shaderBlob{ privateMembers->shaderFactory->MakeVertexShader(shader, length, "main")};

			auto block{ serializer->BeginBlock(shaderBlob->GetBufferSize()) };
			serializer->WriteToBlock(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());			
			
		}


		
		void Renderer::CompilePixelShader(const char *shader, size_t length, SerializationHook* serializer) const
		{
			auto shaderBlob{ privateMembers->shaderFactory->MakePixelShader(shader, length, "main")};

			auto block{ serializer->BeginBlock(shaderBlob->GetBufferSize()) };
			serializer->WriteToBlock(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
			
		}


		
		void Renderer::SerializeRootSignature
		(
			unsigned cbvAmount,
			unsigned srvAmount, 
			unsigned uavAmount,
			unsigned samplerAmount, 
			SerializationHook *serializer
		)
		{
			auto signatureBlob{ privateMembers->signatureFactory.SerializeRootSignature(cbvAmount, srvAmount, uavAmount, samplerAmount) };
			const auto signatureSize{ signatureBlob->GetBufferSize() };

			auto block{ serializer->BeginBlock(sizeof signatureSize + signatureBlob->GetBufferSize() + sizeof samplerAmount) };
			serializer->WriteToBlock(&signatureSize, sizeof signatureSize);
			serializer->WriteToBlock(signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize());
			
			serializer->WriteToBlock(&samplerAmount, sizeof samplerAmount);
			
		}


		
		size_t Renderer::MakeRootSignature(const void *serializedData)
		{
			const auto size{ ExtractSizeFrom(serializedData) };
			auto signatureData
			{
				privateMembers->signatureFactory.MakeRootSignature
				(
					ExtractSignatureFrom(serializedData), 
					size,
					ExtractSamplerCountFrom(serializedData, size)
				)
			};
			
			const auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Signature) };
			privateMembers->registry.RegisterSignature(handle.hash, std::move(signatureData));

			return handle.hash;
			
		}

			SIZE_T Renderer::ExtractSizeFrom(const void *data)
			{
				return *reinterpret_cast<const SIZE_T *>(data);
			
			}

			const unsigned char *Renderer::ExtractSignatureFrom(const void *data)
			{
				return reinterpret_cast<const unsigned char *>(data) + sizeof SIZE_T;
			
			}

			size_t Renderer::ExtractSamplerCountFrom(const void *data, const SIZE_T signatureSize)
			{
				return *reinterpret_cast<const size_t *>
				(
					reinterpret_cast<const unsigned char *>(data)
					+ sizeof SIZE_T
					+ signatureSize
				);
			
			}


		
		size_t Renderer::MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle)
		{
			auto pipelineState{	privateMembers->psoFactory.MakePso(shaders, privateMembers->registry.GetSignature(signatureHandle), pipelineType, privateMembers->vertexLayoutProvider.GetLayoutDesc(vertexLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) };
			
			auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Pso) };
			privateMembers->registry.RegisterPso(handle.hash, pipelineState);

			return handle.hash;
			
		}


		
		bool Renderer::ResourceMustBeRemade(size_t handle)
		{
			return privateMembers->registry.HandleIsInvalid(handle);
			
		}


		
		void Renderer::RetireHandle(const size_t handle)
		{
			privateMembers->handlesToRetire.push_front(handle);
			
		}

		
	}

	
}

