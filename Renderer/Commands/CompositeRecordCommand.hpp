#pragma once
#include "Commands/DX12Command.hpp"
#include "Commands/BindPsoAndSignatureCommand.hpp"
#include <vector>
#include "Shared/PtrTypes.hpp"


namespace Renderer::DX12::Commands
{	
	class CompositeRecordCommand : public DX12Command
	{		
		private: std::vector<UniquePtr<BindPsoAndSignatureCommand>> children;

		private: bool requiresOnCompleted;

		public: CompositeRecordCommand() : requiresOnCompleted{ false }{}		

		
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) final override
		{
			ExecuteOperationOnCompositeResourceReferences(registry, operation);
			for(auto &&cmd : children)
			{				
				cmd->ExecuteOperationOnResourceReferences(registry, operation);
			}

		}		

		public: virtual void ExecuteOperationOnCompositeResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))	{}
			
		public: void Execute(RHA::DX12::CmdList &list, HasQueriableResources &registry) final override
		{
			OnRecordChildrenPre(list, registry);
			for(auto &&cmd : children)
			{
				cmd->RecordPsoBinding(list, registry);
				cmd->RecordSignatureBinding(list, registry);
				cmd->Execute(list, registry);
			}
			OnRecordChildrenPost(list, registry);

		}
				
		public: virtual void OnRecordChildrenPre(RHA::DX12::CmdList &list, HasQueriableResources &registry)	{}

		public: virtual void OnRecordChildrenPost(RHA::DX12::CmdList &list, HasQueriableResources &registry) {}
		

		public: void OnCompleted() final override
		{
			for(auto &&cmd : children)
			{
				cmd->OnCompleted();
			}

		}

		public: bool RequiresOnCompleted() final override
		{
			return requiresOnCompleted;

		}
		
		public: void Add(UniquePtr<BindPsoAndSignatureCommand> &&cmd)
		{
			requiresOnCompleted |= cmd->RequiresOnCompleted();

			children.emplace_back(std::move(cmd));

		}		
		
	};
	

}
