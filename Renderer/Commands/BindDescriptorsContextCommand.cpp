#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"


namespace Renderer::DX12::Commands
{
	BindDescriptorsContextCommand::BindDescriptorsContextCommand(DescriptorMemory &descriptorMemory) : descriptorMemory{ &descriptorMemory }
	{
	}
	

	void BindDescriptorsContextCommand::Execute(DX12CommandProcessor &context)
	{
		if(context.DoesContextEventMatchAny(Renderer::Commands::CommandContextEventFlags::AllBindingsInvalidated | Renderer::Commands::CommandContextEventFlags::InitialContextCommandExecution))
		{
			descriptorMemory->RecordListBinding(&context.GetList());			
		}
		
	}

	
}
