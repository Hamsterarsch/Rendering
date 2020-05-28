#include "Commands/Basic/SetIndexBufferCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetIndexBufferCommand::SetIndexBufferCommand
	(
		const ResourceHandle::t_hash indexBuffer, 
		const size_t byteOffsetToIndices, 
		const size_t numIndices,
		HasQueriableResources &resourceProvider
	)	:
		indexBuffer{ indexBuffer }
	{
		indexView.BufferLocation = resourceProvider.GetResourceGpuAddress(indexBuffer) + byteOffsetToIndices;
		indexView.SizeInBytes = numIndices * sizeof(unsigned);
		indexView.Format = DXGI_FORMAT_R32_UINT;
		
	}


	
	void SetIndexBufferCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, indexBuffer);
		
	}


	
	void SetIndexBufferCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetIndexBuffer(indexView);
		
	}

	
}
