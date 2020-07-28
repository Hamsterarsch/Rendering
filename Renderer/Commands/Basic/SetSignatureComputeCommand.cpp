#include "Commands/Basic/SetSignatureComputeCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetSignatureComputeCommand::SetSignatureComputeCommand(const ResourceHandle::t_hash signature) :
		signature{ signature }
	{
	}


	
	void SetSignatureComputeCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, signature);
		
	}


	
	void SetSignatureComputeCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetComputeSignature(context.GetRegistry().GetSignature(signature));
		context.NotifyCommandContextAbout(Renderer::Commands::CommandContextEvents::ComputeBindingsInvalidated);
		
	}

	
}
