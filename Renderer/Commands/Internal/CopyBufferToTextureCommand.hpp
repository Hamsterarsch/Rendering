#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class CopyBufferToTextureCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash from;

		private: ResourceHandle::t_hash to;

		private: D3D12_PLACED_SUBRESOURCE_FOOTPRINT srcFootprint;
		
		private: unsigned textureSubresourceIndex;

		
		
		public: CopyBufferToTextureCommand
		(
			ResourceHandle::t_hash fromBuffer,
			ResourceHandle::t_hash toTexture,
			const D3D12_PLACED_SUBRESOURCE_FOOTPRINT &srcFootprint,
			unsigned textureSubresourceIndex
		);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
