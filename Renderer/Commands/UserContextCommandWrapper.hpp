#pragma once
#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Shared/PtrTypes.hpp"


namespace Renderer::DX12::Commands
{
	class UserContextCommandWrapper : public BindDescriptorsContextCommand
	{
		private: UniquePtr<Command> userContextCommand;

		

		public: UserContextCommandWrapper(DescriptorMemory &descriptorMemory, UniquePtr<Command> &&userContextCommand)
			: BindDescriptorsContextCommand{ descriptorMemory }, userContextCommand{ std::move(userContextCommand) }
		{			
		}

		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override
		{
			BindDescriptorsContextCommand::ExecuteOperationOnResourceReferences(registry, operation);
			
			userContextCommand->ExecuteOperationOnResourceReferences(registry, operation);
			
		}

		public:	void Execute(DX12CommandProcessor &context) override
		{
			userContextCommand->Execute(context);
			
			BindDescriptorsContextCommand::Execute(context);
			
		}
		
	};

	
}
