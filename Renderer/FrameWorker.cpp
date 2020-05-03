#include "FrameWorker.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/RenderCommand.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


namespace Renderer
{
	namespace DX12
	{
		FrameWorker::FrameWorker
		(
			DeviceResources *resources, 
			Queue *queue,
			ResourceRegistry &masterRegistry,
			WindowSurface &windowSurface, 
			DepthSurface &depthSurface,
			HandleWrapper &&globalBufferHandle,
			bool shouldPresentOnComplete
		) :
			resources{ resources },
			queue{ queue },
			registryMaster{ &masterRegistry },
			registryCopy{ masterRegistry },
			windowSurface{ &windowSurface },
			depthSurface{ &depthSurface },
			commandsRecordedToList{ 0 },
			globalBufferHandle{ std::move(globalBufferHandle) },
			isAllowedToPresent{ shouldPresentOnComplete }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, false, nullptr);

			masterRegistry.AddReference(this->globalBufferHandle);
			registryCopy.AddReference(this->globalBufferHandle);
						
		}


		
		FrameWorker::FrameWorker(FrameWorker &&other) noexcept :
			resources{ std::move(other.resources) },
			queue{ std::move(other.queue) },
			allocator{ std::move(other.allocator) },
			list{ std::move(other.list) },
			fence{ std::move(other.fence) },
			event{ std::move(other.event) },
			commands{ std::move(other.commands) },
			registryMaster{ std::move(other.registryMaster) },
			registryCopy{ std::move(other.registryCopy) },
			windowSurface{ std::move(other.windowSurface) },
			depthSurface{ std::move(other.depthSurface) },
			commandsRecordedToList{ std::move(other.commandsRecordedToList) },
			globalBufferHandle{ std::move(other.globalBufferHandle) }
		{
			other.resources = nullptr;
			other.queue = nullptr;
			other.event = nullptr;		
			other.registryMaster = nullptr;
			other.windowSurface = nullptr;
			other.depthSurface = nullptr;

		}


		
		FrameWorker &FrameWorker::operator=(FrameWorker &&rhs) noexcept
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
			
			registryMaster = std::move(rhs.registryMaster);
			rhs.registryMaster = nullptr;

			registryCopy = std::move(rhs.registryCopy);
			
			windowSurface = std::move(rhs.windowSurface);
			rhs.windowSurface = nullptr;
			
			depthSurface = std::move(rhs.depthSurface);
			rhs.depthSurface = nullptr;

			commandsRecordedToList = std::move(rhs.commandsRecordedToList);

			globalBufferHandle = std::move(rhs.globalBufferHandle);
			
			return *this;
			
		}


		
		FrameWorker::~FrameWorker() noexcept
		{
			UnregisterResources();
			
		}

			void FrameWorker::UnregisterResources()
			{
				if(RegistryIsValid())
				{
					registryCopy.Empty();					
					UnregisterAllCommands();
					registryMaster->RemoveReference(globalBufferHandle);
					registryMaster = nullptr;
					globalBufferHandle.Reset();
				}
			
			}

				void FrameWorker::UnregisterAllCommands()
				{
					for(auto &&cmd : commands)
					{
						cmd->ExecuteOperationOnResourceReferences(registryMaster, &UsesReferences::RemoveReference);
					}				
				
				}


		
		void FrameWorker::AddCommand(UniquePtr<RenderCommand> &&command)
		{			
			command->ExecuteOperationOnResourceReferences(registryMaster, &UsesReferences::AddReference);
			command->ExecuteOperationOnResourceReferences(&registryCopy, &UsesReferences::AddReference);
			registryCopy.AddReference(command->GetPsoHandle());
			registryCopy.AddReference(command->GetSignatureHandle());
			
			commands.emplace_back(std::move(command));
			
		}


		
		int FrameWorker::ExecuteCommands()
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
				
			}
			catch(std::exception &e)
			{
				return 1;
			}

			return 0;
			
		}

			void FrameWorker::RecordRenderTargetPreparations()
			{			
				const auto dsv{ depthSurface->GetHandleCpu() };
				list->RecordClearDsv(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
				
				auto glist{ list->AsGraphicsList() };
				windowSurface->RecordPreparationForRendering(glist.Get());
				windowSurface->RecordPipelineBindings(glist.Get(), &dsv);
			
			}

			void FrameWorker::RecordCommands()
			{
				commandsRecordedToList = 0;
			
				for(auto &&cmd : commands)
				{				
					cmd->RecordFixedCommandState(list.get(), registryCopy, globalBufferHandle);
					
					cmd->Record(list.get(), registryCopy);
					++commandsRecordedToList;			
									
					if(ListCapacityIsReached())
					{
						SubmitCurrentList();
						ResetCurrentList();					
					}
				}
			
			}

				bool FrameWorker::ListCapacityIsReached() const
				{
					return commandsRecordedToList >= recordsPerCommandList;
			
				}

				void FrameWorker::SubmitCurrentList()
				{
					list->StopRecording();

					queue->SubmitCommandList(list.get());
					commandsRecordedToList = 0;
			
				}

				void FrameWorker::ResetCurrentList()
				{
					list->StartRecording(allocator.get());						
					
					const auto dsv{ depthSurface->GetHandleCpu() };
					windowSurface->RecordPipelineBindings(list->AsGraphicsList().Get(), &dsv);
			
				}

			void FrameWorker::SetupCompletionFence()
			{
				fence->Signal(0);
				fence->SetEventOnValue(fenceCmdCompletionValue, event);
				queue->Signal(fenceCmdCompletionValue, fence.get());
			
			}

		

		void FrameWorker::WaitForCompletion()
		{
			WaitForSingleObject(event, INFINITE);

			auto c = allocator->Reset();//todo error handling
			PresentIfAllowed();
			
		}

			void FrameWorker::PresentIfAllowed()
			{
				if(isAllowedToPresent)
				{
					windowSurface->Present();		
				}
			
			}

		
	}
	
	
}
