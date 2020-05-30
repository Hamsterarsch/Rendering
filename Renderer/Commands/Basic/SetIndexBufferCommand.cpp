#include "Commands/Basic/SetIndexBufferCommand.hpp"
#include "StateSettings/TargetHelpers.hpp"


namespace Renderer::DX12::Commands
{
	SetIndexBufferCommand::SetIndexBufferCommand
	(
		const ResourceHandle::t_hash indexBuffer, 
		const size_t byteOffsetToIndices, 
		const size_t numIndices,
		const size_t indexSizeInBytes,
		const Format indexFormat,
		HasQueriableResources &resourceProvider
	)	:
		indexBuffer{ indexBuffer }
	{
		indexView.BufferLocation = resourceProvider.GetResourceGpuAddress(indexBuffer) + byteOffsetToIndices;
		indexView.SizeInBytes = numIndices * indexSizeInBytes;
		indexView.Format = GetTargetValue<DXGI_FORMAT>(indexFormat);
		
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
