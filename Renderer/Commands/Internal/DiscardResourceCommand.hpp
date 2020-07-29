#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class DiscardResourceCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash resource;
				 
		private: unsigned firstSubresourceIndex;

		private: unsigned numSubresourcesToDiscard;

		
		
		public: DiscardResourceCommand(ResourceHandle::t_hash resource, unsigned firstSubresourceIndex, unsigned numSubresourcesToDiscard);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}

