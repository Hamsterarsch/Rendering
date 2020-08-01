#include "Commands/Internal/CopyBufferToTextureCommand.hpp"


namespace Renderer::DX12::Commands
{
	CopyBufferToTextureCommand::CopyBufferToTextureCommand
	(
		const ResourceHandle::t_hash fromBuffer,
		const ResourceHandle::t_hash toTexture,
		const D3D12_PLACED_SUBRESOURCE_FOOTPRINT &srcFootprint,
		const unsigned textureSubresourceIndex
	)
		:
		from{ fromBuffer },
		to{ toTexture },
		srcFootprint{ srcFootprint },
		textureSubresourceIndex{ textureSubresourceIndex }
	{
	}


	
	void CopyBufferToTextureCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{		
		Invoke(registry, operation, from);					
		Invoke(registry, operation, to);
		
	}


	
	void CopyBufferToTextureCommand::Execute(DX12CommandProcessor &context)
	{
		D3D12_TEXTURE_COPY_LOCATION src{};
		src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.pResource = context.GetRegistry().GetResource(from);
		src.PlacedFootprint = srcFootprint;
				
		D3D12_TEXTURE_COPY_LOCATION dst{};
		dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.pResource = context.GetRegistry().GetResource(to);
		dst.SubresourceIndex = textureSubresourceIndex;
						
		context.GetList().RecordCopyTextureRegion(src, dst);
				
	}

	
}
