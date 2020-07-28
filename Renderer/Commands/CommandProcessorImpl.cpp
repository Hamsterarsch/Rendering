#include "Commands/CommandProcessorImpl.hpp"
#include "Commands/DX12Command.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "DX12/Facade.hpp"
#include "CounterFactoryImpl.hpp"


namespace Renderer::DX12::Commands
{
	CommandProcessorImpl::CommandProcessorImpl(RHA::DX12::DeviceResources &resources, RHA::DX12::Queue &queue, ResourceRegistry &registry, CounterFactoryImpl &counterFactory) :
		gpuQueue{ &queue },
		allocator{ RHA::DX12::Facade::MakeCmdAllocator(&resources, D3D12_COMMAND_LIST_TYPE_DIRECT) },
		list{ allocator->AllocateList() },
		fence{ RHA::DX12::Facade::MakeFence(&resources) },
		event{ CreateEvent(nullptr, false, false, nullptr) },
		shouldUpdate{ true },
		commandsExecutedSinceListSubmit{ 0 },
		maxExecutedCommandsPerList{ 50 },
		registry{ &registry },
		currentContextEvent{ Renderer::Commands::CommandContextEvents::Nothing },
		counterFactory{ &counterFactory }
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
								recordedCommands.push_back(std::move(currentContextCommand));							
							}
						}
						currentContextCommand = std::move(bucket.command);
						NotifyCommandContextAbout(Renderer::Commands::CommandContextEvents::AllBindingsInvalidated);
						
						queuedCommands.Pop();
						continue;
						
					}
					
					bucket.command->Execute(*this);
					++commandsExecutedSinceListSubmit;					

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

					recordedCommands.push_back(std::move(bucket.command));						
										
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
				NotifyCommandContextAbout(Renderer::Commands::CommandContextEvents::AllBindingsInvalidated);
		
			}

				void CommandProcessorImpl::NotifyCommandContextAbout(const Renderer::Commands::CommandContextEvents event)
				{
					if(event == Renderer::Commands::CommandContextEvents::Nothing)
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
		//we always need to do this even when commandsRecordedToList is 0, because commands can call this during execution
		
		SubmitList();
				
		fence->SetEventOnValue(fenceCompletionValue, event);
		fence->Signal(fenceCompletionValue, gpuQueue);

		WaitForSingleObject(event, INFINITE);
		fence->Signal(0);
		
		allocator->Reset();
		ResetList();

		{
			std::lock_guard lock{ mutexOutputCommands };
			if(commandsToBeFreed.empty())
			{
				using std::swap;
				swap(recordedCommands, commandsToBeFreed);
				return;
				
			}
			
			while(!recordedCommands.empty())
			{
				commandsToBeFreed.emplace_back(std::move(recordedCommands.back()));
				recordedCommands.pop_back();				
			}			
		}
		
	}


	
	bool CommandProcessorImpl::DoesContextEventMatch(const Renderer::Commands::CommandContextEvents reason) const
	{
		return reason == currentContextEvent;
		
	}


	
	void CommandProcessorImpl::SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command)
	{
		command->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::AddReference);
		queuedCommands.Push({std::move(command)});
		
	}

	
	
	intptr_t CommandProcessorImpl::SubmitExtractableCommand(UniquePtr<::Renderer::Commands::Command> &&command)
	{
		const auto key{ reinterpret_cast<intptr_t>(command.get()) };

		command->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::AddReference);
		queuedCommands.Push({std::move(command), false, key });

		return key;
		
	}


	
	void CommandProcessorImpl::SubmitContextCommand(UniquePtr<::Renderer::Commands::Command> &&command)
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


	
	UniquePtr<::Renderer::Commands::Command> CommandProcessorImpl::ExtractCommand(const intptr_t handle)
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

		for(auto &&cmd : commandsToBeFreed)
		{
			cmd->ExecuteOperationOnResourceReferences(*registry, &UsesReferences::RemoveReference);
		}
		commandsToBeFreed.clear();			
	
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


	
	CounterFactoryImpl &CommandProcessorImpl::GetCounters()
	{
		return *counterFactory;
		
	}

	
}
