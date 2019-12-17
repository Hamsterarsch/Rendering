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
		FrameRenderer::FrameRenderer() :
			resources{ nullptr },
			queue{ nullptr },
			registry{ nullptr }
		{
		}

		FrameRenderer::FrameRenderer
		(
			DeviceResources *resources, 
			Queue *queue,
			ResourceRegistry &registry,
			const DxPtr<ID3D12Resource> &renderTargetTemplate
		) :
			resources{ resources },
			queue{ queue },
			registry{ &registry }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, true, nullptr);

			
			auto rtDesc{ renderTargetTemplate->GetDesc() };			

			viewport.Width = rtDesc.Width;
			viewport.Height = rtDesc.Height;
			viewport.TopLeftX = -(rtDesc.Width / 2.f);
			viewport.TopLeftY = -(rtDesc.Height / 2.f);
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			scissorRect.left = viewport.TopLeftX;
			scissorRect.top = viewport.TopLeftY;
			scissorRect.bottom = viewport.Height / 2.f;
			scissorRect.right = viewport.Width / 2.f;
			
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

		FrameRenderer::FrameRenderer(FrameRenderer &&other) noexcept :
			resources{ std::move(other.resources) },
			queue{ std::move(other.queue) },
			allocator{ std::move(other.allocator) },
			fence{ std::move(other.fence) },
			event{ std::move(other.event) },
			renderTarget{ std::move(other.renderTarget) },
			depthTarget{ std::move(other.depthTarget) },
			rtvHeap{ std::move(other.rtvHeap) },
			dsvHeap{ std::move(other.dsvHeap) },
			commands{ std::move(other.commands) },
			registry{ std::move(other.registry) },
			scissorRect{ std::move(other.scissorRect) },
			viewport{ std::move(other.viewport) }			
		{
			other.resources = nullptr;
			other.queue = nullptr;
			other.event = nullptr;		
			other.registry = nullptr;

		}

		FrameRenderer &FrameRenderer::operator=(FrameRenderer &&rhs) noexcept
		{
			resources = std::move(rhs.resources);
			rhs.resources = nullptr;
			
			queue = std::move(rhs.queue);
			rhs.queue = nullptr;
			
			allocator = std::move(rhs.allocator);
			fence = std::move(rhs.fence);
			
			event = std::move(rhs.event);
			rhs.event = nullptr;
			
			renderTarget = std::move(rhs.renderTarget);
			depthTarget = std::move(rhs.depthTarget);
			rtvHeap = std::move(rhs.rtvHeap);
			dsvHeap = std::move(rhs.dsvHeap);
			commands = std::move(rhs.commands);
			
			registry = std::move(rhs.registry);
			rhs.registry = nullptr;
			
			scissorRect = std::move(rhs.scissorRect);
			viewport = std::move(rhs.viewport);	

			return *this;
			
		}

		FrameRenderer::~FrameRenderer() noexcept
		{
			if(registry != nullptr)
			{
				for(auto &&cmd : commands)
				{
					cmd->ExecuteOperationOnResourceReferences(registry, &ResourceRegistry::RemoveReference);
				}				
			}
			
		}


		
		void FrameRenderer::AddCommand(UniquePtr<RenderCommand> &&command)
		{			
			command->ExecuteOperationOnResourceReferences(registry, &ResourceRegistry::AddReference);			
			commands.emplace_back(std::move(command));
			
		}


		
		void FrameRenderer::ExecuteCommands()
		{
			auto list{ allocator->AllocateList() };
			auto glist{ list->AsGraphicsList() };

			const auto rtv{ rtvHeap->GetHandleCpu(0) };
			const auto dsv{ dsvHeap->GetHandleCpu(0) };

			glist->RSSetScissorRects(1, &scissorRect);
			glist->RSSetViewports(1, &viewport);			
			glist->OMSetRenderTargets(1, &rtv, false, &dsv );
			glist->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
			glist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
			
			size_t recordedCommands{ 0 };
			for(auto &&cmd : commands)
			{				
				glist->SetGraphicsRootSignature(registry->GetSignature(cmd->GetSignatureHandle()));

				cmd->Record(list.get(), *registry);

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

					resetResult = glist->Reset(allocator->GetAllocator().Get(), registry->GetPso(cmd->GetPsoHandle()).Get());

					glist->RSSetScissorRects(1, &scissorRect);
					glist->RSSetViewports(1, &viewport);					
					glist->OMSetRenderTargets(1, &rtv, false, &dsv );
					glist->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
					glist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
				}
				else
				{
					 resetResult = glist->Reset(allocator->GetAllocator().Get(), registry->GetPso(cmd->GetPsoHandle()).Get());					
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
				cmd->ExecuteOperationOnResourceReferences(registry, &ResourceRegistry::RemoveReference);
			}
			commands.clear();
			
			allocator->Reset();
			
		}

		
	}
	
	
}
