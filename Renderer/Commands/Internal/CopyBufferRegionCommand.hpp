#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class CopyBufferRegionCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash src;

		private: ResourceHandle::t_hash dst;

		private: size_t bytesToCopy;

		private: size_t offsetIntoSrcInBytes;
		
		private: size_t offsetIntoDstInBytes;

		
		
		public: CopyBufferRegionCommand
		(
			ResourceHandle::t_hash src,
			ResourceHandle::t_hash dst,
			size_t bytesToCopy,
			size_t offsetIntoSrcInBytes,
			size_t offsetIntoDstInBytes
		);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
