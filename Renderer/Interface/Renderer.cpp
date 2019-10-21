#include "Renderer.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"


namespace Renderer
{
	Renderer::Renderer(HWND outputWindow) :
		inflightFramesAmount{ 1 },
		shouldUpdateRendering{ false }
	{
		resources = RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, true);
		commonQueue = RHA::DX12::Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		outputSurface = RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
		commonAllocator = RHA::DX12::Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

		auto shFactory{ RHA::DX12::Facade::MakeShaderFactory(5,0) };

		auto vertex
		{
			shFactory->MakeVertexShader
			(
				Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.vs").data(),
				"main"
			)
		};

		auto pixel
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
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,
			0
		};
		
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
		inputLayoutDesc.pInputElementDescs = &inputElemDesc;
		inputLayoutDesc.NumElements = 1;
		
		
		psoDesc.InputLayout = inputLayoutDesc;				
		psoDesc.VS = D3D12_SHADER_BYTECODE{ vertex->GetBufferPointer(), vertex->GetBufferSize() };
		psoDesc.PS = D3D12_SHADER_BYTECODE{ pixel->GetBufferPointer(), pixel->GetBufferSize() };
		psoDesc.DepthStencilState.DepthEnable = false;

		
		D3D12_RASTERIZER_DESC rasterDesc{};
		rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterDesc.FrontCounterClockwise = true;
		
		psoDesc.RasterizerState = rasterDesc;


		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //?
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		

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
		D3D12_HEAP_DESC heapDesc{};
		heapDesc.Properties = D3D12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE, D3D12_MEMORY_POOL_L0, 0, 0 };
		heapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

		D3D12_HEAP_PROPERTIES bufferHeapProps{ D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE, D3D12_MEMORY_POOL_L0, 0, 0 };
		
		D3D12_RESOURCE_DESC bufferDesc{};
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		bufferDesc.Width = 9;
		bufferDesc.Height = 1;

		DxPtr<ID3D12Resource> bufferResource;
		auto r5 = resources->GetDevice()->CreateCommittedResource(&bufferHeapProps, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&bufferResource));

		//bufferResource->M
		
		//D3D12_VERTEX_BUFFER_VIEW d{}
		
		auto list
		{
			commonAllocator->AllocateList()
		};

		auto gral{ list->AsGraphicsList() };
		gral->IASetVertexBuffers(0,1,)
		gral->DrawInstanced(3, 1, 0, 0);
		
		updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);

		{
			std::lock_guard<std::mutex> lock{ updaterMutex };
			shouldUpdateRendering = true;			
		}
		updaterCondition.notify_all();
			
	}

		int Renderer::UpdateRendering()
		{
			std::unique_lock<std::mutex> lock{ updaterMutex };
			updaterCondition.wait(lock, [&shouldUpdateRendering = shouldUpdateRendering] { return shouldUpdateRendering; });

			while(shouldUpdateRendering)
			{
				outputSurface->ScheduleBackbufferClear(commonQueue.get());

				//other commands
				
				outputSurface->SchedulePresentation(commonQueue.get());							
			}
			
			return 0;
		
		}

	Renderer::~Renderer()
	{
		{
			std::lock_guard<std::mutex> lock{ updaterMutex };
			shouldUpdateRendering = false;

			updaterHandle.wait();
		}
		
	}


	void Renderer::SubmitFrameInfo()
	{
		
				
	}

	
}

