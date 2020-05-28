#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class SetSignatureGraphicsCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash signature;

		
		
		public: SetSignatureGraphicsCommand(ResourceHandle::t_hash signature);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
