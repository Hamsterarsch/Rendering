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

		struct ActiveFrameData
		{
			FrameRenderer renderer;
			std::future<void> handle;
		};
		
		struct Renderer::PrivateMembers
		{
			HandleFactory handleFactory;
			
			ResourceRegistry registry;
			
			VertexLayoutProvider vertexLayoutProvider;
			
			PsoFactory psoFactory;
			
			RootSignatureFactory signatureFactory;
			
			UniquePtr<ShaderFactory> shaderFactory;
			
			ActiveFrameData activeFrameData;
			
			std::list<FrameRenderer> pendingRenderers;
			
			FrameRenderer commandTargetFrame;

			PrivateMembers(DeviceResources *resources) :
				psoFactory{ resources },
				signatureFactory{ resources },
				shaderFactory{ Facade::MakeShaderFactory(5, 0) }
			{				
			}				
			
		};

		Renderer::Renderer(HWND outputWindow) :
			maxPendingFrames{ 10 },
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
			privateMembers->commandTargetFrame = FrameRenderer{ resources.get(), commonQueue.get(), privateMembers->registry, *outputSurface, *depthSurface };

			
			shouldUpdateRendering = true;			
			updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);
			
				
		}

			int Renderer::UpdateRendering()
			{
				while(shouldUpdateRendering)
				{					
					if(ActiveRendererIsInvalid())
					{
						continue;
					}					
				
					//wait for the active frame to finish
					privateMembers->activeFrameData.handle.wait();
					privateMembers->activeFrameData.renderer.WaitForCompletion();//todo: use wait time to preprocess the next available frame
					
					//todo: renderer retirement reference updates
			
					//if there are pending frames, launch one
					if(ThereArePendingRenderers())
					{
						//launch a new frame
						LaunchFrameRenderer(PopPendingRenderer());
																		
					}

				}

				closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
				closeFence->Signal(1, commonQueue.get());
				WaitForSingleObject(closeEvent, INFINITE);
				
				return 0;
			
			}

				bool Renderer::ActiveRendererIsInvalid()
				{
					std::lock_guard<std::mutex> lock{ frameLaunchMutex };
					return privateMembers->activeFrameData.renderer.IsInvalid();
			
				}

				bool Renderer::ThereArePendingRenderers()
				{
					std::lock_guard<std::mutex> lock{ pendingFramesMutex };

					return !privateMembers->pendingRenderers.empty();
					
				}

				void Renderer::LaunchFrameRenderer(FrameRenderer &&renderer)
				{
					std::lock_guard<std::mutex> lock{ frameLaunchMutex };
								
					privateMembers->activeFrameData.renderer = std::move(renderer); 																
					privateMembers->activeFrameData.handle = std::async(std::launch::async, &FrameRenderer::ExecuteCommands, &privateMembers->activeFrameData.renderer);
			
				}


		
		Renderer::~Renderer()
		{
			{				
				shouldUpdateRendering = false;

				updaterHandle.wait();
				auto f = updaterHandle.get();
				
			}
			
		}


		
		size_t Renderer::MakeAndUploadBufferResource(const void *data, const size_t sizeInBytes)
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

		size_t Renderer::MakeRootSignature(const void *serializedData, size_t dataLength)
		{
			auto *signatureSize{ reinterpret_cast<const SIZE_T *>(serializedData) };
			auto *signaturePtr{ reinterpret_cast<const unsigned char *>(serializedData) + sizeof *signatureSize};

			auto signatureData{ privateMembers->signatureFactory.MakeRootSignature(signaturePtr, *signatureSize) };
			signatureData.samplerAmount = *reinterpret_cast<const size_t *>(signaturePtr + *signatureSize);

			const auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Signature) };
			privateMembers->registry.RegisterSignature(handle.hash, std::move(signatureData));

			return handle.hash;
			
		}

		size_t Renderer::MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle)
		{
			auto pipelineState{	privateMembers->psoFactory.MakePso(shaders, privateMembers->registry.GetSignature(signatureHandle), pipelineType, privateMembers->vertexLayoutProvider.GetLayoutDesc(vertexLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) };
			
			auto handle{ privateMembers->handleFactory.MakeHandle(ResourceTypes::Pso) };
			privateMembers->registry.RegisterPso(handle.hash, pipelineState);

			return handle.hash;
			
		}

		bool Renderer::ResourceHasToBeReloaded(size_t handle)
		{
			return privateMembers->registry.HandleIsInvalid(handle);
			
		}


		void Renderer::RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices)
		{
			privateMembers->commandTargetFrame.AddCommand
			(
				std::make_unique<RenderMeshCommand>(signatureHandle, psoHandle, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices)
			);			

		}

		void Renderer::DispatchFrame()
		{			
			if(ActiveRendererIsInvalid())
			{
				LaunchFrameRenderer(std::move(privateMembers->commandTargetFrame));				
			}
			else
			{
				PushPendingRenderer(std::move(privateMembers->commandTargetFrame));
			}
			
			privateMembers->commandTargetFrame = FrameRenderer{resources.get(), commonQueue.get(), privateMembers->registry, *outputSurface, *depthSurface};
									
			//batch commands where possible

			//create descriptors for each command's resources
			//on the frame renderers portion on the gpu descriptor heap
			//(root signature tables are allowed to overlap unused parts)
			
			//get a frame renderer
			
		}

		FrameRenderer Renderer::PopPendingRenderer()
		{
			std::lock_guard<std::mutex> lock{ pendingFramesMutex };

			auto out{ privateMembers->pendingRenderers.front() };
			privateMembers->pendingRenderers.pop_front();

			return out;
			
		}

		void Renderer::PushPendingRenderer(FrameRenderer &&renderer)
		{
			std::lock_guard<std::mutex> lock{ pendingFramesMutex };

			if(PendingRendererCountIsAtMax())
			{
				privateMembers->pendingRenderers.pop_back();
			}
			
			privateMembers->pendingRenderers.push_back(std::move(renderer));
			
		}

			bool Renderer::PendingRendererCountIsAtMax() const
			{
				return privateMembers->pendingRenderers.size() == maxPendingFrames;
			
			}




		
	}

	
}

