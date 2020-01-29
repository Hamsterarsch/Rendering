#include "FrameRenderer.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceRegistry.hpp"


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
			WindowSurface &windowSurface, 
			DepthSurface &depthSurface
		) :
			resources{ resources },
			queue{ queue },
			registry{ &registry },
			windowSurface{ &windowSurface },
			depthSurface{ &depthSurface }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, true, nullptr);
						
		}

		FrameRenderer::FrameRenderer(FrameRenderer &&other) noexcept :
			resources{ std::move(other.resources) },
			queue{ std::move(other.queue) },
			allocator{ std::move(other.allocator) },
			fence{ std::move(other.fence) },
			event{ std::move(other.event) },
			commands{ std::move(other.commands) },
			registry{ std::move(other.registry) },
			windowSurface{ std::move(other.windowSurface) },
			depthSurface{ std::move(other.depthSurface) }			
		{
			other.resources = nullptr;
			other.queue = nullptr;
			other.event = nullptr;		
			other.registry = nullptr;
			other.windowSurface = nullptr;
			other.depthSurface = nullptr;

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
			
			commands = std::move(rhs.commands);
			
			registry = std::move(rhs.registry);
			rhs.registry = nullptr;
			
			windowSurface = std::move(rhs.windowSurface);
			rhs.windowSurface = nullptr;
			
			depthSurface = std::move(rhs.depthSurface);
			rhs.depthSurface = nullptr;

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
			
			const auto dsv{ depthSurface->GetHandleCpu() };
			glist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
			
			windowSurface->RecordPreparationForRendering(glist.Get());
			windowSurface->RecordPipelineBindings(glist.Get(), &dsv);
						
			size_t recordedCommands{ 0 };
			for(auto &&cmd : commands)
			{				
				glist->SetPipelineState(registry->GetPso(cmd->GetPsoHandle()));
				glist->SetGraphicsRootSignature(registry->GetSignature(cmd->GetSignatureHandle()));
				
				cmd->Record(list.get(), *registry);
								
				if(recordedCommands >= recordsPerCommandList)
				{
					const auto closeResult{ glist->Close() };
					if(FAILED(closeResult))
					{
						//error indication
						return;
					}

					queue->SubmitCommandList(list.get());
					recordedCommands = 0;
					
					auto resetResult = glist->Reset(allocator->GetAllocator().Get(), registry->GetPso(cmd->GetPsoHandle()));
					if(FAILED(resetResult))
					{
						//error indidcation
						return;
					}
					
					glist->SetGraphicsRootSignature(registry->GetSignature(cmd->GetSignatureHandle()));
					windowSurface->RecordPipelineBindings(glist.Get(), &dsv);
										
				}
				++recordedCommands;				
			}

			windowSurface->RecordPreparationForPresenting(glist.Get());
			
			const auto closeResult{ glist->Close() };
			if(SUCCEEDED(closeResult))
			{
				queue->SubmitCommandList(list.get());
			}

			fence->Signal(0);
			fence->SetEventOnValue(1, event);
			queue->Signal(1, fence.get());

			const auto result{ queue->GetQueue()->Wait(fence->GetFence().Get(), 2) };
			if(FAILED(result))
			{
				throw;
			}
			
		}

		void FrameRenderer::WaitForCompletion()
		{
			WaitForSingleObject(event, INFINITE);

			windowSurface->Present();

			fence->Signal(2);
			
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
