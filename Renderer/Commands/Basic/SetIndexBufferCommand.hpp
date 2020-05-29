#pragma once
#include "Commands/DX12Command.hpp"
#include "StateSettings/FormatTargets.hpp"


namespace Renderer::DX12::Commands
{
	class SetIndexBufferCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash indexBuffer;
		
		private: D3D12_INDEX_BUFFER_VIEW indexView;
		
						 		
		
		public: SetIndexBufferCommand
		(
			ResourceHandle::t_hash indexBuffer,
			size_t byteOffsetToIndices,
			size_t numIndices,
			size_t indexSizeInBytes,
			t_format_target indexFormat,
			HasQueriableResources &resourceProvider
		);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}
