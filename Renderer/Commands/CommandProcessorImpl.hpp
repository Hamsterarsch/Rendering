#pragma once
#include "Commands/DX12CommandProcessor.hpp"
#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"
#include "Shared/PtrTypes.hpp"
#include "Shared/Types/Containers/QueueConcurrent.hpp"

#include <future>

namespace RHA::DX12
{
	class Fence;
	class DeviceResources;
	class Queue;	
}

namespace Renderer::DX12{ class ResourceRegistry; }


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
			UniquePtr<DX12Command> command;
			
			bool isContextCommand;

			intptr_t extractHandle;

			Bucket() : Bucket{ {}, false } {}

			Bucket(UniquePtr<DX12Command> &&command) : Bucket{ std::move(command), false } {}

			Bucket(UniquePtr<DX12Command> &&command, bool isContextCommand) : Bucket{ std::move(command), isContextCommand, 0 } {}
			
			Bucket(UniquePtr<DX12Command> &&command, bool isContextCommand, intptr_t extractHandle) : command{ std::move(command) }, isContextCommand{ isContextCommand }, extractHandle{ extractHandle } {}
			
		};
		
		private: QueueConcurrent<Bucket> queuedCommands;
				 		
		private: UniquePtr<DX12Command> currentContextCommand;

		private: std::vector<UniquePtr<DX12Command>> executedCommands;

		private: std::unordered_map<intptr_t, UniquePtr<DX12Command>> extractableCommands;

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

		private: CommandContextEvents currentContextEvent;

		

		public: CommandProcessorImpl(RHA::DX12::DeviceResources &resources, RHA::DX12::Queue &queue, ResourceRegistry &registry);

			private: ThreadResult Update();

				private: bool ListCapacityIsReached() const;
		
				private: void SubmitList();

				private: void ResetList();

		public: void NotifyCommandContextAbout(CommandContextEvents event) override;
					
						private: void ExecuteContextCommand();

		public: ~CommandProcessorImpl() override;
		
		
		public: RHA::DX12::CmdList &GetList() override;

		public: HasQueriableResources &GetRegistry() override;
						
		public: void SubmitAndWaitForGpuWork() override;

		public: bool ShouldExecuteContextCommandFor(CommandContextEvents reason) const override;


		public: void SubmitCommand(UniquePtr<DX12Command> &&command);

		public: intptr_t SubmitExtractableCommand(UniquePtr<DX12Command> &&command);
		
		public: void SubmitContextCommand(UniquePtr<DX12Command> &&command);


		public: void WaitForCommand(intptr_t handle);

		public: UniquePtr<DX12Command> ExtractCommand(intptr_t handle);

		public: void FreeExecutedCommands();

		public: void WaitForIdle();

		public: void PropagateExceptions();


		
		
		
		
	};
	
	
}
