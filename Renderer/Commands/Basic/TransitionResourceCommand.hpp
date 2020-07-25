#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class TransitionResourceCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash resource;

		private: D3D12_RESOURCE_STATES from;

		private: D3D12_RESOURCE_STATES to;

		
		public: TransitionResourceCommand(ResourceHandle::t_hash resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}


