#include "FrameRenderer.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


namespace Renderer
{
	namespace DX12
	{
		FrameRenderer::FrameRenderer() :
			resources{ nullptr },
			queue{ nullptr },
			registry{ nullptr },
			windowSurface{ nullptr },
			depthSurface{ nullptr }
		{
		}

		FrameRenderer::FrameRenderer
		(
			DeviceResources *resources, 
			Queue *queue,
			ResourceRegistry &registry,
			WindowSurface &windowSurface, 
			DepthSurface &depthSurface,
			HandleWrapper &&globalBufferHandle
		) :
			resources{ resources },
			queue{ queue },
			registry{ &registry },
			windowSurface{ &windowSurface },
			depthSurface{ &depthSurface },
			commandsRecordedToList{ 0 },
			globalBufferHandle{ std::move(globalBufferHandle) }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, false, nullptr);
			registry.AddReference(this->globalBufferHandle);
						
		}


		
		FrameRenderer::FrameRenderer(FrameRenderer &&other) noexcept :
			resources{ std::move(other.resources) },
			queue{ std::move(other.queue) },
			allocator{ std::move(other.allocator) },
			list{ std::move(other.list) },
			fence{ std::move(other.fence) },
			event{ std::move(other.event) },
			commands{ std::move(other.commands) },
			registry{ std::move(other.registry) },
			windowSurface{ std::move(other.windowSurface) },
			depthSurface{ std::move(other.depthSurface) },
			commandsRecordedToList{ std::move(other.commandsRecordedToList) },
			globalBufferHandle{ std::move(other.globalBufferHandle) }
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
			list = std::move(rhs.list);
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

			commandsRecordedToList = std::move(rhs.commandsRecordedToList);

			globalBufferHandle = std::move(rhs.globalBufferHandle);
			
			return *this;
			
		}


		
		FrameRenderer::~FrameRenderer() noexcept
		{
			UnregisterResources();
			
		}

			void FrameRenderer::UnregisterResources()
			{
				if(RegistryIsValid())
				{
					UnregisterAllCommands();
					registry->RemoveReference(globalBufferHandle);
					registry = nullptr;
				}
			
			}

				void FrameRenderer::UnregisterAllCommands()
				{
					for(auto &&cmd : commands)
					{
						cmd->ExecuteOperationOnResourceReferences(registry, &ResourceRegistry::RemoveReference);
					}				
				
				}


		
		void FrameRenderer::AddCommand(UniquePtr<RenderCommand> &&command)
		{			
			command->ExecuteOperationOnResourceReferences(registry, &ResourceRegistry::AddReference);			
			commands.emplace_back(std::move(command));
			
		}


		
		int FrameRenderer::ExecuteCommands()
		{
			try
			{
				list = allocator->AllocateList();
				
				RecordRenderTargetPreparations();									
				RecordCommands();

				windowSurface->RecordPreparationForPresenting(list->AsGraphicsList().Get());
				
				list->StopRecording();
				queue->SubmitCommandList(list.get());
				
				SetupCompletionFence();
				
				queue->Wait(fenceQueueReleaseValue, fence.get());
			}
			catch(std::exception &e)
			{
				return 1;
			}

			return 0;
			
		}

			void FrameRenderer::RecordRenderTargetPreparations()
			{			
				const auto dsv{ depthSurface->GetHandleCpu() };
				list->RecordClearDsv(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
				
				auto glist{ list->AsGraphicsList() };
				windowSurface->RecordPreparationForRendering(glist.Get());
				windowSurface->RecordPipelineBindings(glist.Get(), &dsv);
			
			}

			void FrameRenderer::RecordCommands()
			{
				commandsRecordedToList = 0;
			
				for(auto &&cmd : commands)
				{				
					RecordFixedCommandState(*cmd);
					
					cmd->Record(list.get(), *registry);
					++commandsRecordedToList;			
									
					if(ListCapacityIsReached())
					{
						SubmitCurrentList();
						ResetCurrentList();					
					}
				}
			
			}

				void FrameRenderer::RecordFixedCommandState(RenderCommand &cmd) const
				{
					list->RecordSetPipelineState(registry->GetPso(cmd.GetPsoHandle()));
					list->RecordSetGraphicsSignature(registry->GetSignature(cmd.GetSignatureHandle()));
					list->AsGraphicsList()->SetGraphicsRootConstantBufferView(0, registry->GetResourceGPUVirtualAddress(globalBufferHandle));
				}

				bool FrameRenderer::ListCapacityIsReached() const
				{
					return commandsRecordedToList >= recordsPerCommandList;
			
				}

				void FrameRenderer::SubmitCurrentList()
				{
					list->StopRecording();

					queue->SubmitCommandList(list.get());
					commandsRecordedToList = 0;
			
				}

				void FrameRenderer::ResetCurrentList()
				{
					list->StartRecording(allocator.get());						
					
					const auto dsv{ depthSurface->GetHandleCpu() };
					windowSurface->RecordPipelineBindings(list->AsGraphicsList().Get(), &dsv);
			
				}

			void FrameRenderer::SetupCompletionFence()
			{
				fence->Signal(0);
				fence->SetEventOnValue(fenceCmdCompletionValue, event);
				queue->Signal(fenceCmdCompletionValue, fence.get());
			
			}

		

		void FrameRenderer::WaitForCompletion()
		{
			WaitForSingleObject(event, INFINITE);

			auto c = allocator->Reset();
			windowSurface->Present();

			fence->Signal(fenceQueueReleaseValue);
			
		}

		
	}
	
	
}
