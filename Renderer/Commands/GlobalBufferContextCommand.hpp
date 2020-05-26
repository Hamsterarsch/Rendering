#pragma once
#include "Commands/BindDescriptorsContextCommand.hpp"
#include "Resources/HandleWrapper.hpp"
#include <utility>


namespace Renderer::DX12::Commands
{
	class GlobalBufferContextCommand : public BindDescriptorsContextCommand
	{
		private: HandleWrapper buffer;


		public: GlobalBufferContextCommand(DescriptorMemory &descriptorMemory, HandleWrapper &&buffer) : BindDescriptorsContextCommand{ descriptorMemory }, buffer{ std::move(buffer) } {}
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override
		{
			BindDescriptorsContextCommand::ExecuteOperationOnResourceReferences(registry, operation);
			
			(registry.*operation)(buffer);
			
		}

		public:	void Execute(DX12CommandProcessor &context) override
		{
			BindDescriptorsContextCommand::Execute(context);

			if(context.ShouldExecuteContextCommandFor(CommandContextEvents::GraphicsSignatureChanged))
			{
				context.GetList().RecordSetGraphicsSignatureCbv(0, context.GetRegistry().GetResourceGpuAddress(buffer));				
			}

			if(context.ShouldExecuteContextCommandFor(CommandContextEvents::ComputeSignatureChanged))
			{
				context.GetList().RecordSetComputeSignatureCbv(0, context.GetRegistry().GetResourceGpuAddress(buffer));				
			}
			
		}
		
	};

	
}
