#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"


namespace Renderer::DX12::Commands
{
	BindDescriptorsContextCommand::BindDescriptorsContextCommand(DescriptorMemory &descriptorMemory) : descriptorMemory{ &descriptorMemory }
	{
	}
	

	void BindDescriptorsContextCommand::Execute(DX12CommandProcessor &context)
	{
		if(context.DoesContextEventMatch(Renderer::Commands::CommandContextEvents::AllBindingsInvalidated))
		{
			descriptorMemory->RecordListBinding(&context.GetList());			
		}
		
	}

	
}
