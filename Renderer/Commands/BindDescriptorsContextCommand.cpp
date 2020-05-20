#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"


namespace Renderer::DX12::Commands
{
	BindDescriptorsContextCommand::BindDescriptorsContextCommand(DescriptorMemory &descriptorMemory) : descriptorMemory{ &descriptorMemory }
	{
	}
	

	void BindDescriptorsContextCommand::Execute(CommandProcessor &context)
	{
		if(context.ShouldExecuteContextCommandFor(CommandContextEvents::CommandListChanged))
		{
			descriptorMemory->RecordListBinding(&context.GetList());			
		}
		
	}

	
}
