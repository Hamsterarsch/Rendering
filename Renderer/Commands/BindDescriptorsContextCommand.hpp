#pragma once
#include "Commands/Command.hpp"


namespace Renderer::DX12 { class DescriptorMemory; }

namespace Renderer::DX12::Commands
{
	class BindDescriptorsContextCommand : public Command
	{
		private: DescriptorMemory *descriptorMemory;



		public: BindDescriptorsContextCommand(DescriptorMemory &descriptorMemory);
		
		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		void virtual Execute(CommandProcessor &context) override;
				
	};

	
}
