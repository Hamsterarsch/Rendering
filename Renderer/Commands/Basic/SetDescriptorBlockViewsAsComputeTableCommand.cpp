#include "Commands/Basic/SetDescriptorBlockViewsAsComputeTableCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetDescriptorBlockViewsAsComputeTableCommand::SetDescriptorBlockViewsAsComputeTableCommand(const ResourceHandle::t_hash descriptorBlock, const unsigned parameterIndex) :
		descriptorBlock{ descriptorBlock },
		parameterIndex{ parameterIndex }
	{
	}


	
	void SetDescriptorBlockViewsAsComputeTableCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, descriptorBlock);
		
	}


	
	void SetDescriptorBlockViewsAsComputeTableCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetComputeSignatureTable(parameterIndex, context.GetRegistry().GetDescriptorAllocator(descriptorBlock).GetCurrentTableStartForView());
		
	}

	
}
