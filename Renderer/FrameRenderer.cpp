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
			event = CreateEvent(nullptr, false, true, nullptr);

			
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

			dsvHeap = Facade::MakeDescriptorHeap(resources, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);			
			resources->GetDevice()->CreateDepthStencilView(depthTarget.Get(), nullptr, dsvHeap->GetHandleCpu(0));

			rtvHeap = Facade::MakeDescriptorHeap(resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false);
			resources->GetDevice()->CreateRenderTargetView(renderTarget.Get(), nullptr, rtvHeap->GetHandleCpu(0));

			
		}

		FrameRenderer::~FrameRenderer() noexcept = default;


		
		void FrameRenderer::AddCommand(UniquePtr<RenderCommand> &&command)
		{			
			command->ExecuteOperationOnResourceReferences(&registry, &ResourceRegistry::AddReference);			
			commands.emplace_back(std::move(command));
			
		}


		
		void FrameRenderer::ExecuteCommands()
		{
			auto list{ allocator->AllocateList() };
			auto glist{ list->AsGraphicsList() };

			const auto rtv{ rtvHeap->GetHandleCpu(0) };
			const auto dsv{ dsvHeap->GetHandleCpu(0) };
			
			glist->OMSetRenderTargets(1, &rtv, false, &dsv );
			glist->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
			glist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
			
			size_t recordedCommands{ 0 };
			for(auto &&cmd : commands)
			{				
				glist->SetGraphicsRootSignature(registry.GetSignatureDataRef(cmd->GetSignatureHandle()).Get());

				cmd->Record(list.get(), registry);

				HRESULT resetResult{ S_OK };
				if(recordedCommands >= recordsPerCommandList)
				{
					const auto closeResult{ glist->Close() };
					if(FAILED(closeResult))
					{
						//error indication
						return;
					}

					queue->SubmitCommandList(list.get());

					resetResult = glist->Reset(allocator->GetAllocator().Get(), registry.GetPso(cmd->GetPsoHandle()).Get());
					
					glist->OMSetRenderTargets(1, &rtv, false, &dsv );
					glist->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
					glist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
				}
				else
				{
					 resetResult = glist->Reset(allocator->GetAllocator().Get(), registry.GetPso(cmd->GetPsoHandle()).Get());					
				}
				++recordedCommands;

				if(FAILED(resetResult))
				{
					//error indidcation
					return;
				}
			}

			const auto closeResult{ glist->Close() };
			if(SUCCEEDED(closeResult))
			{
				queue->SubmitCommandList(list.get());
			}

			fence->Signal(0);
			fence->SetEventOnValue(1, event);
			queue->Signal(1, fence.get());
			
		}

		void FrameRenderer::WaitForCompletion()
		{
			WaitForSingleObject(event, INFINITE);
			
		}

		void FrameRenderer::Reinitialize()
		{
			for(auto &&cmd : commands)
			{
				cmd->ExecuteOperationOnResourceReferences(&registry, &ResourceRegistry::RemoveReference);
			}
			commands.clear();
			
			allocator->Reset();
			
		}

		
	}
	
	
}
