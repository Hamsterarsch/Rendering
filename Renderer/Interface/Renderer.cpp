#include "Renderer.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Filesystem/Conversions.hpp"


struct vertex
{
	float x, y, z;
};

namespace Renderer
{
	Renderer::Renderer(HWND outputWindow) :
		inflightFramesAmount{ 1 },
		shouldUpdateRendering{ false }
	{
		resources = RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_11_0, true);
		commonQueue = RHA::DX12::Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		outputSurface = RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
		commonAllocator = RHA::DX12::Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		closeFence = RHA::DX12::Facade::MakeFence(resources.get());
		closeEvent = CreateEvent(nullptr, false, false, nullptr);
		
		auto shFactory{ RHA::DX12::Facade::MakeShaderFactory(5,0) };

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

		
		D3D12_RASTERIZER_DESC rasterDesc{};
		rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
		rasterDesc.FrontCounterClockwise = true;
		
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
		upHeap = RHA::DX12::Facade::MakeUploadHeap(resources.get(), 256);
			   		
		D3D12_HEAP_PROPERTIES bufferHeapProps{ D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE, D3D12_MEMORY_POOL_L0, 0, 0 };


		vertex vertices[3]{ {0.25, 0.75, 0 }, {0.75, 0.75, 0}, {0.5, 0.25, 0} };		
		unsigned indices[3]{ 0,1,2 };
		
		D3D12_VERTEX_BUFFER_VIEW vertexView{};
		vertexView.BufferLocation = upHeap->CopyDataToUploadAddress(vertices, sizeof(vertices), sizeof(float));
		vertexView.SizeInBytes = sizeof(vertices);
		vertexView.StrideInBytes = sizeof(vertex);

		D3D12_INDEX_BUFFER_VIEW indexView{};
		indexView.BufferLocation = upHeap->CopyDataToUploadAddress(indices, sizeof(indices), sizeof(unsigned));
		indexView.SizeInBytes = sizeof(indices);
		indexView.Format = DXGI_FORMAT_R32_UINT;
			   		

		
		list = commonAllocator->AllocateList();
		

		auto gral{ list->AsGraphicsList() };
		gral->SetPipelineState(pipeline.Get());
		gral->SetGraphicsRootSignature(signature.Get());
		outputSurface->RecordRasterizerBindings(gral.Get());
		gral->IASetVertexBuffers(0, 1, &vertexView);
		gral->IASetIndexBuffer(&indexView);
		gral->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gral->DrawIndexedInstanced(3, 1, 0, 0, 0);
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

				commonQueue->SubmitCommandList(list.get());
				//other commands
				
				outputSurface->SchedulePresentation(commonQueue.get());							
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


	void Renderer::SubmitFrameInfo()
	{
		
				
	}

	
}

