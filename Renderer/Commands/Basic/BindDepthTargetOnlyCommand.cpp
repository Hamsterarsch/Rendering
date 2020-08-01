#include "Commands/Basic/BindDepthTargetOnlyCommand.hpp"
#include "DX12/WindowSurface.hpp"


namespace Renderer::DX12::Commands
{
	BindDepthTargetOnlyCommand::BindDepthTargetOnlyCommand(const ResourceHandle::t_hash depthTarget)
		:		
		depthTargetDescriptor{ depthTarget }
	{
	}


	
	void BindDepthTargetOnlyCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, depthTargetDescriptor);
		
	}


	
	void BindDepthTargetOnlyCommand::Execute(DX12CommandProcessor &context)
	{
		auto descriptor{ context.GetRegistry().GetRawDescriptor(depthTargetDescriptor) };
		context.GetList().RecordSetRenderTargets(0, nullptr, false, &descriptor);
		
	}

	
}
