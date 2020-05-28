#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class SetVertexBufferCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash vertexBuffer;
		
		private: D3D12_VERTEX_BUFFER_VIEW vertexView;
		
						 		
		
		public: SetVertexBufferCommand
		(
			ResourceHandle::t_hash vertexBuffer,
			size_t byteOffsetToVertices,
			size_t numVertices,
			size_t vertexStrideInBytes,
			HasQueriableResources &resourceProvider
		);
		
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
