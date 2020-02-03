#include "Renderer.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"


#include "Resources/ResourceFactory.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/HandleFactory.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"

#include "FrameRenderer.hpp"
#include "RenderMeshCommand.hpp"
#include "Resources/Pso/VertexLayoutProvider.hpp"
#include "Shared/Types/Containers/QueueConcurrent.hpp"


#if _DEBUG
	constexpr bool enableDebugLayers = true;
#else
	constexpr bool enableDebugLayers = false;
#endif

	
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
			
			QueueConcurrent<FrameRenderer> frames;

			std::future<int> activeFrameHandle;
						
			std::list<UniquePtr<RenderCommand>> commandsToDispatch;

			PrivateMembers(DeviceResources *resources) :
				psoFactory{ resources },
				signatureFactory{ resources },
				shaderFactory{ Facade::MakeShaderFactory(5, 0) }
			{				
			}				
			
		};

		Renderer::Renderer(HWND outputWindow) :
			shouldUpdateRendering{ false },
			privateMembers{ nullptr }
		{
			resources = Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, enableDebugLayers);
			commonQueue = Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);			

			outputSurface = Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
			depthSurface = Facade::MakeDepthSurface(resources.get(), outputSurface->GetResourceTemplate()->GetDesc());
			
			closeFence = Facade::MakeFence(resources.get());
			closeEvent = CreateEvent(nullptr, false, false, nullptr);
						
			resourceFactory = std::make_unique<ResourceFactory>
			(
				resources.get(),
				commonQueue.get(),
				std::make_unique<ResourceMemory>(resources.get(), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 15, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS)
			);			

			privateMembers = std::make_unique<PrivateMembers>(resources.get());					   			
						
			shouldUpdateRendering = true;			
			updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);
							
		}

			int Renderer::UpdateRendering()
			{
				try
				{
					while(shouldUpdateRendering)
					{						
						if(privateMembers->frames.IsEmpty())
						{
							UpdateIdle();
							continue;
						}
						ExecuteNextFrame();										
					}

					WaitForIdleQueue();
				}
				catch(std::exception &e)
				{
					return 1;
				}
			
				return 0;
			
			}

				void Renderer::UpdateIdle()
				{					
				}

				void Renderer::ExecuteNextFrame()
				{
					auto frame{ privateMembers->frames.Pop() };
																	
					privateMembers->activeFrameHandle = std::async(std::launch::async, &FrameRenderer::ExecuteCommands, &frame);
					if(privateMembers->activeFrameHandle.get())
					{
						throw;
					}

					frame.WaitForCompletion();
			
				}

				void Renderer::WaitForIdleQueue()
				{
					closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
					closeFence->Signal(1, commonQueue.get());
					WaitForSingleObject(closeEvent, INFINITE);
			
				}

	
		
		Renderer::~Renderer()
		{			
			shouldUpdateRendering = false;					
			auto result = updaterHandle.get();
							
		}



		void Renderer::DispatchFrame()
		{
			if(NextFrameSlotIsOccupied())
			{
				AbortDispatch();
				return;
			}
			
			privateMembers->frames.Push
			(
				MakeFrameFromCommands()
			);
			
		}

			bool Renderer::NextFrameSlotIsOccupied() const
			{
				return privateMembers->frames.Size() >= 2;
			
			}

			void Renderer::AbortDispatch()
			{
				privateMembers->commandsToDispatch.clear();
			
			}

			FrameRenderer Renderer::MakeFrameFromCommands()
			{
				FrameRenderer renderer{ resources.get(), commonQueue.get(), privateMembers->registry, *outputSurface, *depthSurface };
				for(auto &&cmd : privateMembers->commandsToDispatch)
				{
					renderer.AddCommand(std::move(cmd));
				}
				privateMembers->commandsToDispatch.clear();

				return renderer;
			
			}

		

		void Renderer::RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices)
		{
			privateMembers->commandsToDispatch.emplace_back
			(
				std::make_unique<RenderMeshCommand>(signatureHandle, psoHandle, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices)
			);			

		}

		
		
		size_t Renderer::MakeBuffer(const void *data, const size_t sizeInBytes)
		{
			auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Buffer) };

			auto allocation
			{
				resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
			};

			privateMembers->registry.RegisterResource(handle, std::move(allocation));

			return handle.hash;
			
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
		
		
	}

	
}

