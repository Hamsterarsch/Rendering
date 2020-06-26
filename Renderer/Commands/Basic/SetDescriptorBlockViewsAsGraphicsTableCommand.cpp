#include "Commands/Basic/SetDescriptorBlockViewsAsGraphicsTableCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetDescriptorBlockViewsAsGraphicsTableCommand::SetDescriptorBlockViewsAsGraphicsTableCommand(const ResourceHandle::t_hash descriptorBlock, const unsigned parameterIndex) :
		descriptorBlock{ descriptorBlock },
		parameterIndex{ parameterIndex }
	{
	}


	
	void SetDescriptorBlockViewsAsGraphicsTableCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, descriptorBlock);
		
	}


	
	void SetDescriptorBlockViewsAsGraphicsTableCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetGraphicsSignatureTable(parameterIndex, context.GetRegistry().GetDescriptorAllocator(descriptorBlock).GetCurrentTableStartForView());
		
	}

	
}
