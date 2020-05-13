#include "FrameWorker.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/RenderCommand.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Commands/CommandPrepareSurfaceForRendering.hpp"
#include "Commands/CommandPrepareSurfaceForPresent.h"


namespace Renderer
{
	namespace DX12
	{
		FrameWorker::FrameWorker
		(
			DeviceResources *resources, 
			Queue *queue,
			DescriptorMemory &descriptors,
			ResourceRegistry &masterRegistry,
			const RenderSurface &surfaceToPresent,
			HandleWrapper &&globalBufferHandle,
			bool shouldPrepareSurface
		) :
			resources{ resources },
			queue{ queue },
			descriptors{ &descriptors },
			registryMaster{ &masterRegistry },
			registryCopy{ masterRegistry },
			surfaceToPresent{ surfaceToPresent },
			commandsRecordedToList{ 0 },
			globalBufferHandle{ std::move(globalBufferHandle) },
			shouldPrepareSurface{ shouldPrepareSurface }			
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			fence = Facade::MakeFence(resources);
			event = CreateEvent(nullptr, false, false, nullptr);

			masterRegistry.AddReference(this->globalBufferHandle);
			registryCopy.AddReference(this->globalBufferHandle);

			if(shouldPrepareSurface)
			{
				AddCommand(std::make_unique<CommandPrepareSurfaceForRendering>(surfaceToPresent));
			}
						
		}


		
		FrameWorker::FrameWorker(FrameWorker &&other) noexcept :
			resources{ std::move(other.resources) },
			queue{ std::move(other.queue) },
			descriptors{ std::move(other.descriptors) },
			allocator{ std::move(other.allocator) },
			list{ std::move(other.list) },
			fence{ std::move(other.fence) },
			event{ std::move(other.event) },
			commands{ std::move(other.commands) },
			registryMaster{ std::move(other.registryMaster) },
			registryCopy{ std::move(other.registryCopy) },
			surfaceToPresent{ std::move(other.surfaceToPresent) },
			commandsRecordedToList{ std::move(other.commandsRecordedToList) },
			globalBufferHandle{ std::move(other.globalBufferHandle) }			
		{	
			other.registryMaster = nullptr;			

		}


		
		FrameWorker &FrameWorker::operator=(FrameWorker &&rhs) noexcept
		{
			UnregisterResources();
			
			resources = std::move(rhs.resources);						
			queue = std::move(rhs.queue);
			descriptors = std::move(rhs.descriptors);
			allocator = std::move(rhs.allocator);
			list = std::move(rhs.list);
			fence = std::move(rhs.fence);			
			event = std::move(rhs.event);						
			commands = std::move(rhs.commands);
			
			registryMaster = std::move(rhs.registryMaster);
			rhs.registryMaster = nullptr;

			registryCopy = std::move(rhs.registryCopy);			
			surfaceToPresent = std::move(rhs.surfaceToPresent);
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
			command->AddPsoReference(registryCopy);
			command->AddSignatureReference(registryCopy);
			
			commands.emplace_back(std::move(command));
			
		}



		int FrameWorker::ExecuteCommands()
		{
			try
			{
				list = allocator->AllocateList();
				descriptors->RecordListBinding(list.get());

				if(shouldPrepareSurface)
				{					
					AddCommand(std::make_unique<CommandPrepareSurfaceForPresent>(surfaceToPresent));					
				}
				
				RecordCommands();
												
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


					//surfaceToPresent.RecordBindSurfaces(*list);
					descriptors->RecordListBinding(list.get());
					
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
				surfaceToPresent.Present();				
			
			}

		UniquePtr<RenderCommand> FrameWorker::ExtractCommand(size_t index)
		{
			commands.at(0).swap
			(
				commands.at(commands.size()-1)
			);

			auto out{ std::move(commands.at(commands.size()-1)) };
			commands.pop_back();

			return out;
			
		}


		void FrameWorker::ExecuteCommandPostGpuWork()
		{
			for(auto &&cmd : commands)
			{
				cmd->ExecutePostGpuWork();
			}
			
		}

		
	}
	
	
}
