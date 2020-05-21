#include "Commands/CommandProcessorImpl.hpp"
#include "Commands/DX12Command.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "DX12/Facade.hpp"

namespace Renderer::DX12::Commands
{
	CommandProcessorImpl::CommandProcessorImpl(RHA::DX12::DeviceResources &resources, RHA::DX12::Queue &queue, ResourceRegistry &registry) :
		gpuQueue{ &queue },
		allocator{ RHA::DX12::Facade::MakeCmdAllocator(&resources, D3D12_COMMAND_LIST_TYPE_DIRECT) },
		list{ allocator->AllocateList() },
		fence{ RHA::DX12::Facade::MakeFence(&resources) },
		event{ CreateEvent(nullptr, false, false, nullptr) },
		shouldUpdate{ true },
		commandsExecutedSinceListSubmit{ 0 },
		maxExecutedCommandsPerList{ 50 },
		registry{ &registry },
		currentContextEvent{ CommandContextEvents::Nothing }
	{		
		updaterHandle = std::async(std::launch::async, &CommandProcessorImpl::Update, this);
		
	}

		CommandProcessorImpl::ThreadResult CommandProcessorImpl::Update()
		{
			try
			{
				while(shouldUpdate)
				{
					queuedCommands.WaitForItems();
					if(!shouldUpdate)
					{
						break;
						
					}
					
					auto bucket{ queuedCommands.ExtractFront() };
					
					if(bucket.isContextCommand)
					{						
						{
							std::lock_guard lock{ mutexOutputCommands };

							if(currentContextCommand)
							{
								executedCommands.push_back(std::move(currentContextCommand));							
							}
						}
						currentContextCommand = std::move(bucket.command);
						NotifyCommandContextAbout(CommandContextEvents::CommandListChanged);
						
						queuedCommands.Pop();
						continue;
						
					}
					
					bucket.command->Execute(*this);

					if(bucket.extractHandle > 0)
					{
						SubmitAndWaitForGpuWork();

						std::unique_lock lock{ mutexOutputCommands };
						extractableCommands[bucket.extractHandle] = std::move(bucket.command);
						lock.unlock();
						extractableCommandsSignal.notify_all();

						queuedCommands.Pop();
						continue;
						
					}

					{
						std::lock_guard lock{ mutexOutputCommands };
						executedCommands.push_back(std::move(bucket.command));						
					}
					
					++commandsExecutedSinceListSubmit;					
					if(ListCapacityIsReached())
					{
						SubmitList();
						ResetList();
					}
					queuedCommands.Pop();
				}
			}
			catch(std::exception &e)
			{
				return ThreadResult{ 1, e };
			}
		
			return ThreadResult{ 0, {}};
		
		}

			bool CommandProcessorImpl::ListCapacityIsReached() const
			{
				return commandsExecutedSinceListSubmit >= maxExecutedCommandsPerList;
		
			}

			void CommandProcessorImpl::SubmitList()
			{
				list->StopRecording();

				gpuQueue->SubmitCommandList(list.get());
				commandsExecutedSinceListSubmit = 0;
			}
	
			void CommandProcessorImpl::ResetList()
			{
				list = allocator->AllocateList();
				NotifyCommandContextAbout(CommandContextEvents::CommandListChanged);
		
			}

				void CommandProcessorImpl::NotifyCommandContextAbout(const CommandContextEvents event)
				{
					if(event == CommandContextEvents::Nothing)
					{
						return;
					}
					
					currentContextEvent = event;
					ExecuteContextCommand();
					
				}
	
					void CommandProcessorImpl::ExecuteContextCommand()
					{
						if(currentContextCommand)
						{
							currentContextCommand->Execute(*this);
						}
		
					}


	
	CommandProcessorImpl::~CommandProcessorImpl()
	{
		try
		{		
			WaitForIdle();
			shouldUpdate = false;
			queuedCommands.ReleaseWaitingThreads();

			if(updaterHandle.valid())
			{
				updaterHandle.wait();					
			}
		}
		catch(...){}
		
	}

	

	RHA::DX12::CmdList &CommandProcessorImpl::GetList()
	{
		return *list;
		
	}


	
	HasQueriableResources &CommandProcessorImpl::GetRegistry()
	{		
		return *registry;
		
	}


	
	void CommandProcessorImpl::SubmitAndWaitForGpuWork()
	{
		SubmitList();
				
		fence->SetEventOnValue(fenceCompletionValue, event);
		fence->Signal(fenceCompletionValue, gpuQueue);

		WaitForSingleObject(event, INFINITE);
		fence->Signal(0);
		
		allocator->Reset();
		ResetList();
		
	}


	
	bool CommandProcessorImpl::ShouldExecuteContextCommandFor(const CommandContextEvents reason) const
	{
		return reason == currentContextEvent;
		
	}


	
	void CommandProcessorImpl::SubmitCommand(UniquePtr<DX12Command> &&command)
	{
		command->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::AddReference);
		queuedCommands.Push({std::move(command)});
		
	}

	
	
	intptr_t CommandProcessorImpl::SubmitExtractableCommand(UniquePtr<DX12Command> &&command)
	{
		const auto key{ reinterpret_cast<intptr_t>(command.get()) };

		command->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::AddReference);
		queuedCommands.Push({std::move(command), false, key });

		return key;
		
	}


	
	void CommandProcessorImpl::SubmitContextCommand(UniquePtr<DX12Command> &&command)
	{
		command->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::AddReference);
		queuedCommands.Push({std::move(command), true});
		
	}



	void CommandProcessorImpl::WaitForCommand(const intptr_t handle)
	{
		std::unique_lock lock{ mutexOutputCommands };
		
		if(extractableCommands.find(handle) == extractableCommands.end())
		{
			extractableCommandsSignal.wait(lock, [&excommands = extractableCommands, h = handle](){ return excommands.find(h) != excommands.end(); });			
		}		
		
	}


	
	UniquePtr<DX12Command> CommandProcessorImpl::ExtractCommand(const intptr_t handle)
	{
		PropagateExceptions();
		
		std::lock_guard lock{ mutexOutputCommands };
		
		auto out{ std::move(extractableCommands.at(handle)) };
		extractableCommands.erase(handle);
		
		out->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::RemoveReference);
		return out;
		
	}


	
	void CommandProcessorImpl::FreeExecutedCommands()
	{
		std::lock_guard lock{ mutexOutputCommands };

		for(auto &&cmd : executedCommands)
		{
			cmd->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::RemoveReference);
		}
		executedCommands.clear();			
	
	}

	

	void CommandProcessorImpl::WaitForIdle()
	{
		queuedCommands.WaitForEmpty();
		PropagateExceptions();
		
	}

		void CommandProcessorImpl::PropagateExceptions()
		{
			if(updaterHandle.wait_for(std::chrono::seconds{0}) == std::future_status::ready)
			{
				const auto result{ updaterHandle.get() };
				if(result.result)
				{
					throw result.exception;
				}
			}
			
		}

	
}
