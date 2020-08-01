#include "Commands/Internal/CopyBufferRegionCommand.hpp"


namespace Renderer::DX12::Commands
{
	CopyBufferRegionCommand::CopyBufferRegionCommand
	(
		const ResourceHandle::t_hash from,
		const ResourceHandle::t_hash to,
		const size_t bytesToCopy,
		const size_t offsetIntoSrcInBytes,
		const size_t offsetIntoDstInBytes
	)
		:
		src{ from },
		dst{ to },
		bytesToCopy{ bytesToCopy },
		offsetIntoSrcInBytes{ offsetIntoSrcInBytes },
		offsetIntoDstInBytes{ offsetIntoDstInBytes }
	{
	}


	
	void CopyBufferRegionCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		Invoke(registry, operation, src);
		Invoke(registry, operation, dst);
		
	}


	
	void CopyBufferRegionCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordCopyBufferRegion
		(
			context.GetRegistry().GetResource(dst),
			offsetIntoDstInBytes,
			context.GetRegistry().GetResource(src),
			offsetIntoSrcInBytes,
			bytesToCopy
		);
		
	}

	
}
