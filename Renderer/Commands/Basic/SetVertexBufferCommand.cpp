#include "Commands/Basic/SetVertexBufferCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetVertexBufferCommand::SetVertexBufferCommand
	(
		const ResourceHandle::t_hash vertexBuffer,
		const size_t byteOffsetToVertices,
		const size_t numVertices,
		const size_t vertexStrideInBytes,
		HasQueriableResources &resourceProvider
	)	:
		vertexBuffer{ vertexBuffer }
	{
		vertexView.BufferLocation = resourceProvider.GetResourceGpuAddress(vertexBuffer) + byteOffsetToVertices;
		vertexView.SizeInBytes = numVertices * vertexStrideInBytes;
		vertexView.StrideInBytes = vertexStrideInBytes;
		
	}

	void SetVertexBufferCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, vertexBuffer);
		
	}


	
	void SetVertexBufferCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetVertexBuffer(vertexView);
		context.GetList().RecordSetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
	}

	
}
