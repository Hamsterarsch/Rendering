#include "Commands/Basic/SetSignatureGraphicsCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetSignatureGraphicsCommand::SetSignatureGraphicsCommand(const ResourceHandle::t_hash signature) :
		signature{ signature }
	{
	}


	
	void SetSignatureGraphicsCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, signature);
		
	}


	
	void SetSignatureGraphicsCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetGraphicsSignature(context.GetRegistry().GetSignature(signature));
		context.NotifyCommandContextAbout(Renderer::Commands::CommandContextEventFlags::GraphicsBindingsInvalidated);
		
	}

	
}
