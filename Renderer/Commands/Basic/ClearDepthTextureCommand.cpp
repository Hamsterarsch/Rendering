#include "Commands/Basic/ClearDepthTextureCommand.hpp"


namespace Renderer::DX12::Commands
{
	ClearDepthTextureCommand::ClearDepthTextureCommand(const ResourceHandle::t_hash depthTargetDescriptor)
		:		
		depthTargetDescriptor{ depthTargetDescriptor }
	{
	}


	
	void ClearDepthTextureCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, depthTargetDescriptor);
		
	}


	
	void ClearDepthTextureCommand::Execute(DX12CommandProcessor &context)
	{
		const auto descriptor{ context.GetRegistry().GetRawDescriptor(depthTargetDescriptor) };
		context.GetList().RecordClearDsv(descriptor, D3D12_CLEAR_FLAG_STENCIL | D3D12_CLEAR_FLAG_DEPTH, 1, 0);
		
	}

	
}
