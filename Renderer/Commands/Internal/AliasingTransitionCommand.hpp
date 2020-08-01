#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class AliasingTransitionCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash from;

		private: ResourceHandle::t_hash to;


		
		public: AliasingTransitionCommand
		(
			ResourceHandle::t_hash from,
			ResourceHandle::t_hash to			
		);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
