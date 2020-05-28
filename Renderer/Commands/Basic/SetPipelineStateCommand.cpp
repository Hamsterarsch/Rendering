#include "Commands/Basic/SetPipelineStateCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetPipelineStateCommand::SetPipelineStateCommand(const ResourceHandle::t_hash pipeline) :
		pipeline{ pipeline }
	{
	}


	
	void SetPipelineStateCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, pipeline);
		
	}


	
	void SetPipelineStateCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetPipelineState(context.GetRegistry().GetPso(pipeline));
				
	}

	
}
