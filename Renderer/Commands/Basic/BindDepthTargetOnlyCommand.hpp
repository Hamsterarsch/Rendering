#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class BindDepthTargetOnlyCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash depthTargetDescriptor;


		
		public: BindDepthTargetOnlyCommand(ResourceHandle::t_hash depthTarget);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}

