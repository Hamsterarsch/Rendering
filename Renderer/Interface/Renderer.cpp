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

#include <queue>

#if _DEBUG
	constexpr bool enableDebugLayers = true;
#else
	constexpr bool enableDebugLayers = false;
#endif



struct vertex
{
	float x, y, z;
};

namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;

		struct InflightData
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
			std::list<InflightData> inflightFrameRenderers;
			std::queue<FrameRenderer> finishedRenderers, pendingRenderers;
			FrameRenderer currentRenderer;

			PrivateMembers(DeviceResources *resources) :
				psoFactory{ resources },
				signatureFactory{ resources },
				shaderFactory{ Facade::MakeShaderFactory(5, 0) }
			{				
			}				
			
		};
		
		struct TriangleData
		{
			D3D12_VERTEX_BUFFER_VIEW vertexView;
			D3D12_INDEX_BUFFER_VIEW indexView;
		};
		
		Renderer::Renderer(HWND outputWindow) :
			inflightFramesAmount{ 1 },
			shouldUpdateRendering{ false },
			privateMembers{ nullptr }
		{
			resources = Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, enableDebugLayers);
			commonQueue = Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);			
			commonAllocator = Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

			copyQueue = Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_COPY, true);
			copyAllocator = Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_COPY);
			
			outputSurface = Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
			
			closeFence = Facade::MakeFence(resources.get());
			closeEvent = CreateEvent(nullptr, false, false, nullptr);

			copyFence = Facade::MakeFence(resources.get());
			copyEvent = CreateEvent(nullptr, false, false, nullptr);
			
			//data = std::make_unique<TriangleData>();
			resourceFactory = std::make_unique<ResourceFactory>(resources.get(), commonQueue.get(), std::make_unique<ResourceMemory>(resources.get(), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 15, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS));			

			privateMembers = std::make_unique<PrivateMembers>(resources.get());

			
			/*
			auto vs
			{
				privateMembers->shaderFactory->MakeVertexShader
				(
					Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.vs").data(),
					"main"
				)
			};

			auto ps
			{
				privateMembers->shaderFactory->MakePixelShader
				(
					Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.ps").data(),
					"main"
				)
			};


			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

			
			D3D12_INPUT_ELEMENT_DESC inputElemDesc
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,
				0
			};
			
			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
			inputLayoutDesc.pInputElementDescs = &inputElemDesc;
			inputLayoutDesc.NumElements = 1;
			
			
			psoDesc.InputLayout = inputLayoutDesc;				
			psoDesc.VS = D3D12_SHADER_BYTECODE{ vs->GetBufferPointer(), vs->GetBufferSize() };
			psoDesc.PS = D3D12_SHADER_BYTECODE{ ps->GetBufferPointer(), ps->GetBufferSize() };
			psoDesc.DepthStencilState.DepthEnable = false;
			psoDesc.DepthStencilState.StencilEnable = false;

			psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//this is important 
			
			
			D3D12_RASTERIZER_DESC rasterDesc{};
			rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.DepthClipEnable = true;
			
			psoDesc.RasterizerState = rasterDesc;


			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //?
			psoDesc.SampleDesc.Count = 1;
			psoDesc.SampleDesc.Quality = 0;
			psoDesc.SampleMask = UINT_MAX;
			

			D3D12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc{};
			signatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
			signatureDesc.Desc_1_1 = D3D12_ROOT_SIGNATURE_DESC1{};
			signatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			
			DxPtr<ID3DBlob> signatureBlob, errorBlob;

			auto r1 =
			D3D12SerializeVersionedRootSignature(&signatureDesc, &signatureBlob, &errorBlob);
			
			auto r2 =
			resources->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&signature));

			psoDesc.pRootSignature = signature.Get();
					

			auto r3 =
			resources->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline));
			
			
			//
			struct
			{
				vertex vertices[3]{ {-0.75, -0.75, 0 }, {0.75, -0.75, 0}, {0, 0.75, 0} };
				unsigned indices[3]{ 0,1,2 };
			} meshdata;

			meshBufferAllocation = std::make_unique<ResourceAllocation>
			(
				resourceFactory->MakeBufferWithData(&meshdata, sizeof(meshdata), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
			);
						
			data->vertexView.BufferLocation = meshBufferAllocation->resource->GetGPUVirtualAddress();
			data->vertexView.SizeInBytes = sizeof(meshdata.vertices);
			data->vertexView.StrideInBytes = sizeof(vertex);
			
			data->indexView.BufferLocation = meshBufferAllocation->resource->GetGPUVirtualAddress() + sizeof(meshdata.vertices);
			data->indexView.SizeInBytes = sizeof(meshdata.indices);
			data->indexView.Format = DXGI_FORMAT_R32_UINT;

			
			list = commonAllocator->AllocateList();
			auto gral{ list->AsGraphicsList() };
			gral->Close();		
			*/

			updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);

			{
				std::lock_guard<std::mutex> lock{ updaterMutex };
				shouldUpdateRendering = true;			
			}
			updaterCondition.notify_all();
				
		}

			int Renderer::UpdateRendering()
			{
				{
					std::unique_lock<std::mutex> lock{ updaterMutex };
					updaterCondition.wait(lock, [&shouldUpdateRendering = shouldUpdateRendering] { return shouldUpdateRendering; });
					lock.unlock();
				}
			
				while(shouldUpdateRendering)
				{
					if(privateMembers->pendingRenderers.empty())
					{
						continue;
					}
					
					//wait for the oldest pending frame to finish
					privateMembers->pendingRenderers.front().WaitForCompletion();
					
					//copy from frame to output
					{
						auto list{ copyAllocator->AllocateList() };					
						outputSurface->ScheduleCopyToBackbuffer(copyQueue.get(), list.get(), privateMembers->pendingRenderers.front().GetRenderTargetRef());

						copyFence->SetEventOnValue(1, copyEvent);
						copyQueue->Signal(1, copyFence.get());

						WaitForSingleObject(copyEvent, INFINITE);
					}
					privateMembers->pendingRenderers.pop();
					copyAllocator->Reset();
					
					//swap output
					outputSurface->SchedulePresentation(commonQueue.get());
					
					//while the current inflight amount is smaller than the maximum
					//and there are pending frames
					while(privateMembers->inflightFrameRenderers.size() < inflightFramesAmount && privateMembers->pendingRenderers.size() > 0)
					{
						//launch pending frames till max
						LaunchFrameRenderer(std::move(privateMembers->pendingRenderers.front()));
						privateMembers->pendingRenderers.pop();
												
					}

					/*
					outputSurface->ScheduleBackbufferClear(commonQueue.get());

					
					auto gral{ list->AsGraphicsList() };
					const auto d = gral->Reset(commonAllocator->GetAllocator().Get(), pipeline.Get());
					if(FAILED(d))
					{
						throw 1;
					}
					
					outputSurface->RecordPipelineBindings(gral.Get());
					gral->SetGraphicsRootSignature(signature.Get());
					gral->IASetVertexBuffers(0, 1, &data->vertexView);
					gral->IASetIndexBuffer(&data->indexView);
					gral->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					gral->DrawIndexedInstanced(3, 1, 0, 0, 0);
					const auto r = gral->Close();
					if(FAILED(r))
					{
						throw 1;
					}
					commonQueue->SubmitCommandList(list.get());
					
					
					outputSurface->SchedulePresentation(commonQueue.get());

					//makeshift prevention of out of memory
					closeFence->SetEventOnValue(1, closeEvent);
					closeFence->Signal(1, commonQueue.get());
					WaitForSingleObject(closeEvent, INFINITE);
					closeFence->Signal(0);
					
					auto c = commonAllocator->GetAllocator()->Reset();
					*/
				}

				closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
				closeFence->Signal(1, commonQueue.get());
				WaitForSingleObject(closeEvent, INFINITE);
				
				return 0;
			
			}

				void Renderer::LaunchFrameRenderer(FrameRenderer &&renderer)
				{
					privateMembers->inflightFrameRenderers.emplace_back();
					privateMembers->inflightFrameRenderers.back().renderer = std::move(renderer); 
																
					privateMembers->inflightFrameRenderers.back().handle = std::async(std::launch::async, &FrameRenderer::ExecuteCommands, &privateMembers->inflightFrameRenderers.back().renderer);
			
				}

		Renderer::~Renderer()
		{
			{
				std::lock_guard<std::mutex> lock{ updaterMutex };
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
			privateMembers->currentRenderer.AddCommand
			(
				std::make_unique<RenderMeshCommand>(signatureHandle, psoHandle, meshHandle, byteOffsetToIndices, sizeInBytes - byteOffsetToIndices)
			);			

		}

		void Renderer::DispatchFrame()
		{
			if(privateMembers->inflightFrameRenderers.size() < inflightFramesAmount)
			{
				LaunchFrameRenderer(std::move(privateMembers->currentRenderer));				
			}
			else
			{
				privateMembers->pendingRenderers.push(std::move(privateMembers->currentRenderer));

				while(privateMembers->pendingRenderers.size() > inflightFramesAmount)
				{
					privateMembers->pendingRenderers.pop();
				}				
			}


			privateMembers->currentRenderer = FrameRenderer{resources.get(), commonQueue.get(), privateMembers->registry, outputSurface->GetResourceTemplate()};
			//throw if all frame renderers are occupied
						
			//batch commands where possible

			//create descriptors for each command's resources
			//on the frame renderers portion on the gpu descriptor heap
			//(root signature tables are allowed to overlap unused parts)
			
			//get a frame renderer
			
		}

		void Renderer::SubmitFrameInfo()
		{
			
					
		}

		
	}

	
}

