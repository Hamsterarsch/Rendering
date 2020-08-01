#include "Commands/Basic/TransitionResourceCommand.hpp"


namespace Renderer::DX12::Commands
{
	TransitionResourceCommand::TransitionResourceCommand
	(
		const ResourceHandle::t_hash resource, 
		const D3D12_RESOURCE_STATES from, 
		const D3D12_RESOURCE_STATES to
	)	:
		resource{ resource },
		from{ from },
		to{ to }
	{
	}


	
	void TransitionResourceCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences::* operation)(size_t))
	{
		Invoke(registry, operation, resource);
		
	}


	
	void TransitionResourceCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordBarrierTransition(context.GetRegistry().GetResource(resource), from, to);
		
	}

	
}
