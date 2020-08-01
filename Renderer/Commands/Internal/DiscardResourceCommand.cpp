#include "Commands/Internal/DiscardResourceCommand.hpp"


namespace Renderer::DX12::Commands
{
	DiscardResourceCommand::DiscardResourceCommand
	(
		const ResourceHandle::t_hash resource,
		const unsigned firstSubresourceIndex,
		const unsigned numSubresourcesToDiscard
	)
		:		
		resource{ resource },
		firstSubresourceIndex{ firstSubresourceIndex },
		numSubresourcesToDiscard{ numSubresourcesToDiscard }
	{
	}


	
	void DiscardResourceCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, resource);
		
	}


	
	void DiscardResourceCommand::Execute(DX12CommandProcessor &context)
	{
		D3D12_DISCARD_REGION region{ 0, nullptr, firstSubresourceIndex, numSubresourcesToDiscard };		
		
		context.GetList().RecordDiscardResource(context.GetRegistry().GetResource(resource), region);
		
	}

	
}
