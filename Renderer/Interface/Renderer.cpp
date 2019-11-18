#include "Renderer.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"


#include "Resources/ResourceFactory.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/SerialFactory.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceRegistry.hpp"


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

		struct Renderer::PrivateMembers
		{
			SerialFactory bufferSerialFactory;			
			ResourceRegistry registry;
			
		};
		
		struct TriangleData
		{
			D3D12_VERTEX_BUFFER_VIEW vertexView;
			D3D12_INDEX_BUFFER_VIEW indexView;
		};
		
		Renderer::Renderer(HWND outputWindow) :
			inflightFramesAmount{ 1 },
			shouldUpdateRendering{ false },
			privateMembers{ std::make_unique<PrivateMembers>() }
		{
			resources = Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, enableDebugLayers);
			commonQueue = Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
			outputSurface = Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
			commonAllocator = Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
			closeFence = Facade::MakeFence(resources.get());
			closeEvent = CreateEvent(nullptr, false, false, nullptr);
			data = std::make_unique<TriangleData>();
			resourceFactory = std::make_unique<ResourceFactory>(resources.get(), commonQueue.get(), std::make_unique<ResourceMemory>(resources.get(), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 15, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS));			
			auto shFactory{ Facade::MakeShaderFactory(5,0) };

			auto vs
			{
				shFactory->MakeVertexShader
				(
					Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.vs").data(),
					"main"
				)
			};

			auto ps
			{
				shFactory->MakePixelShader
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
				}

				closeFence->GetFence()->SetEventOnCompletion(1, closeEvent);
				closeFence->Signal(1, commonQueue.get());
				WaitForSingleObject(closeEvent, INFINITE);
				
				return 0;
			
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

		size_t Renderer::MakeBufferResourceAndHandle(const void *data, const size_t sizeInBytes)
		{
			ResourceHandle handle{ ResourceTypes::Buffer, privateMembers->bufferSerialFactory.GetNextSerial() };

			auto allocation
			{
				resourceFactory->MakeBufferWithData(data, sizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER)
			};

			privateMembers->registry.RegisterResource(handle, std::move(allocation));

			return handle.hash;
			
		}


		void Renderer::RenderMesh(size_t bufferHandle, size_t sizeInBytes, size_t offsetToIndices)
		{

			
		}

		void Renderer::SubmitFrameInfo()
		{
			
					
		}

		
	}

	
}

