#include "Commands/Internal/AliasingTransitionCommand.hpp"


namespace Renderer::DX12::Commands
{
	AliasingTransitionCommand::AliasingTransitionCommand
	(
		const ResourceHandle::t_hash from,
		const ResourceHandle::t_hash to
	)
		:
		from{ from },
		to{ to }		
	{
	}


	
	void AliasingTransitionCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, from);					
		Invoke(registry, operation, to);
		
	}


	
	void AliasingTransitionCommand::Execute(DX12CommandProcessor &context)
	{
		ID3D12Resource *fromResource{ nullptr };
		if(from)
		{
			fromResource = context.GetRegistry().GetResource(from);
		}
		
		context.GetList().RecordBarrierAliasing(fromResource, context.GetRegistry().GetResource(to));
				
	}

	
}
