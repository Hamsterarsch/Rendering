#include "Commands/Basic/SetDescriptorBlockViewsGraphicsCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetDescriptorBlockViewsGraphicsCommand::SetDescriptorBlockViewsGraphicsCommand(const ResourceHandle::t_hash descriptorBlock) :
		descriptorBlock{ descriptorBlock }
	{
	}


	
	void SetDescriptorBlockViewsGraphicsCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, descriptorBlock);
		
	}


	
	void SetDescriptorBlockViewsGraphicsCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetGraphicsSignatureTable(2, context.GetRegistry().GetDescriptorAllocator(descriptorBlock).GetCurrentTableStartForView());
		
	}

	
}
