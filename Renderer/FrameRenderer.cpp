#include "FrameRenderer.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	namespace DX12
	{
		FrameRenderer::FrameRenderer
		(
			DeviceResources *resources, 
			Queue *queue,
			ResourceRegistry &registry,
			const DxPtr<ID3D12Resource> &renderTargetTemplate
		) :
			resources{ resources },
			queue{ queue },
			registry{ registry }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, false, nullptr);

			
			auto rtDesc{ renderTargetTemplate->GetDesc() };			

			D3D12_HEAP_PROPERTIES properties{};
			properties.Type =  D3D12_HEAP_TYPE_DEFAULT;
			properties.CPUPageProperty =  D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			properties.CreationNodeMask =  D3D12_MEMORY_POOL_UNKNOWN;
						
			{
						
			D3D12_CLEAR_VALUE clear{};
			clear.Format = rtDesc.Format;
			clear.Color[0] = 0;
			clear.Color[1] = 0;
			clear.Color[2] = 0;
			clear.Color[3] = 1;

			auto r =
			resources->GetDevice()->CreateCommittedResource
			(
				&properties,
				D3D12_HEAP_FLAG_NONE,
				&rtDesc,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				&clear,
				IID_PPV_ARGS(&renderTarget)
			);

			}

			D3D12_RESOURCE_DESC dsDesc{ rtDesc };
			dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			D3D12_CLEAR_VALUE clear{};
			clear.Format = dsDesc.Format;
			clear.DepthStencil.Depth = 1;
			clear.DepthStencil.Stencil = 0;

			auto r =
			resources->GetDevice()->CreateCommittedResource
			(
				&properties,
				D3D12_HEAP_FLAG_NONE,
				&dsDesc,
				D3D12_RESOURCE_STATE_RENDER_TARGET | D3D12_RESOURCE_STATE_DEPTH_WRITE | D3D12_RESOURCE_STATE_DEPTH_READ,
				&clear,
				IID_PPV_ARGS(&depthTarget)
			);

			auto dsvHeap = Facade::MakeDescriptorHeap(resources, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);			
			resources->GetDevice()->CreateDepthStencilView(depthTarget.Get(), nullptr, dsvHeap->GetHandleCpu(0));

			auto rtvHeap = Facade::MakeDescriptorHeap(resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false);
			resources->GetDevice()->CreateRenderTargetView(renderTarget.Get(), nullptr, rtvHeap->GetHandleCpu(0));
			
			

		}

		FrameRenderer::~FrameRenderer() noexcept = default;


		
		void FrameRenderer::AddCommand(UniquePtr<RenderCommand> &&command)
		{
			command->RegisterResourceReferences(registry);			
			commands.emplace_back(std::move(commands));
			
		}


		
		void FrameRenderer::ExecuteCommands()
		{
			auto list{ allocator->AllocateList() };
			auto glist{ list->AsGraphicsList() };
			
			for(auto &&cmd : commands)
			{
				const auto resetResult{ glist->Reset(allocator->GetAllocator().Get(), registry.GetPso(cmd->GetPsoHandle()).Get()) };
				if(FAILED(resetResult))
				{
					//error indidcation
					break;
				}
				
				glist->OMSetRenderTargets()
				glist->SetGraphicsRootSignature(registry.GetSignature(cmd->GetSignatureHandle()).Get());
				
				UniquePtr<void> persistentData{ nullptr };
				cmd->Record(list.get(), registry, persistentData);

				if(persistentData)
				{
					persistentCommandData.emplace_back(std::move(persistentData));
				}				
			}
			
		}

		

		void FrameRenderer::Reinitialize()
		{
			WaitForSingleObject(event, INFINITE);			
			
			allocator->Reset();
			
		}

		
	}
	
	
}
