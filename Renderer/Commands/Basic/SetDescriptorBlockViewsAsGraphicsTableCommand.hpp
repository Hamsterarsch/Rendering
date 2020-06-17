#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class SetDescriptorBlockViewsAsGraphicsTableCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash descriptorBlock;

		private: unsigned parameterIndex;
		
						 		
		
		public: SetDescriptorBlockViewsAsGraphicsTableCommand(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex);
		
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}
