#pragma once
#include "Commands/DX12CommandProcessor.hpp"
#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"
#include "Shared/PtrTypes.hpp"
#include "Shared/Types/Containers/QueueConcurrent.hpp"
#include <future>
#include "Commands/Command.hpp"


namespace RHA::DX12
{
	class Fence;
	class DeviceResources;
	class Queue;	
}

namespace Renderer::DX12
{	
	class ResourceRegistry;
}


namespace Renderer::DX12::Commands
{
	class DX12Command;
	
	class CommandProcessorImpl final : public DX12CommandProcessor
	{		
		private: RHA::DX12::Queue *gpuQueue;

		private: UniquePtr<RHA::DX12::CmdAllocator> allocator;

		private: UniquePtr<RHA::DX12::CmdList> list;

		private: UniquePtr<RHA::DX12::Fence> fence;

		private: static constexpr size_t fenceCompletionValue{ 1 };

		private: HANDLE event;

		private: struct Bucket
		{
			UniquePtr<::Renderer::Commands::Command> command;
			
			bool isContextCommand;

			intptr_t extractHandle;

			Bucket() : Bucket{ {}, false } {}

			Bucket(UniquePtr<::Renderer::Commands::Command> &&command) : Bucket{ std::move(command), false } {}

			Bucket(UniquePtr<::Renderer::Commands::Command> &&command, bool isContextCommand) : Bucket{ std::move(command), isContextCommand, 0 } {}
			
			Bucket(UniquePtr<::Renderer::Commands::Command> &&command, bool isContextCommand, intptr_t extractHandle) : command{ std::move(command) }, isContextCommand{ isContextCommand }, extractHandle{ extractHandle } {}
			
		};
		
		private: QueueConcurrent<Bucket> queuedCommands;
				 		
		private: UniquePtr<::Renderer::Commands::Command> currentContextCommand;

		private: std::vector<UniquePtr<::Renderer::Commands::Command>> recordedCommands;

		private: std::vector<UniquePtr<::Renderer::Commands::Command>> commandsToBeFreed;

		private: std::unordered_map<intptr_t, UniquePtr<::Renderer::Commands::Command>> extractableCommands;

		private: std::mutex mutexOutputCommands;

		private: std::condition_variable extractableCommandsSignal;
				 				 
		private: struct ThreadResult
		{
			int result;
			std::exception exception;
		};

		private: std::future<ThreadResult> updaterHandle;

		private: bool shouldUpdate;

		private: size_t commandsExecutedSinceListSubmit;

		private: size_t maxExecutedCommandsPerList;

		private: ResourceRegistry *registry;

		private: Renderer::Commands::CommandContextEventFlags currentContextEventFlags;

		private: CounterFactoryImpl *counterFactory;

		private: bool isProcessingCommandContextNotification;
		

		
		public: CommandProcessorImpl(RHA::DX12::DeviceResources &resources, RHA::DX12::Queue &queue, ResourceRegistry &registry, CounterFactoryImpl &counterFactory);

			private: ThreadResult Update();

				private: bool ListCapacityIsReached() const;
		
				private: void SubmitList();

				private: void ResetList();

		public: void NotifyCommandContextAbout(Renderer::Commands::CommandContextEventFlags event) override;
					
			private: void ExecuteContextCommand();

		public: ~CommandProcessorImpl() override;
		
		
		public: RHA::DX12::CmdList &GetList() override;

		public: HasQueriableResources &GetRegistry() override;
						
		public: void SubmitAndWaitForGpuWork() override;

		public: bool DoesContextEventMatchAll(Renderer::Commands::CommandContextEventFlags eventFlags) const override;

		public: bool DoesContextEventMatchAny(Renderer::Commands::CommandContextEventFlags eventFlags) const override;
		

		public: void SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command);

		public: intptr_t SubmitExtractableCommand(UniquePtr<::Renderer::Commands::Command> &&command);
		
		public: void SubmitContextCommand(UniquePtr<::Renderer::Commands::Command> &&command);


		public: void WaitForCommand(intptr_t handle);

		public: UniquePtr<::Renderer::Commands::Command> ExtractCommand(intptr_t handle);

		public: void FreeExecutedCommands();

		public: void WaitForIdle();

		public: void PropagateExceptions();


		public: CounterFactoryImpl &GetCounters() override;

	};
	
	
}
