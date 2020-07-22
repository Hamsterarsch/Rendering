#include "Commands/Basic/BindRenderTargetsCommand.hpp"
#include "DX12/WindowSurface.hpp"


namespace Renderer::DX12::Commands
{
		BindRenderTargetsCommand::BindRenderTargetsCommand
	(
		const ResourceHandle::t_hash windowSurface,
		const ResourceHandle::t_hash depthTarget
	)
		:
		windowSurface{ windowSurface },
		depthTargetDescriptor{ depthTarget }
	{
	}


	
	void BindRenderTargetsCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, windowSurface);
		Invoke(registry, operation, depthTargetDescriptor);
		
	}


	
	void BindRenderTargetsCommand::Execute(DX12CommandProcessor &context)
	{
		const auto depthDescriptor{ context.GetRegistry().GetRawDescriptor(depthTargetDescriptor) };
		context.GetRegistry().GetSurface(windowSurface)->RecordPipelineBindings(context.GetList(), &depthDescriptor);
		
	}

	
}
