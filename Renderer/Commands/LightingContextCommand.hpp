#pragma once
#include "Commands/GlobalBufferContextCommand.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"


namespace Renderer::DX12{ class ResourceRegistry; }


namespace Renderer::DX12::Commands
{
	class LightingContextCommand final : public Command
	{
		private: GlobalBufferContextCommand gbCommand;

		private: DescriptorAllocator lightingDescriptors;
				 		
		private: size_t tileGridData;

		private: size_t lightsList;

		private: size_t relevantLightsList;

		private: size_t mapToRelevantLights;
		


		public: LightingContextCommand
		(
			DescriptorMemory &descriptorMemory,
			HandleWrapper &&buffer,
			size_t lightingSignature,
			size_t tileGridData,
			size_t lightsList,
			size_t relevantLightsList,
			size_t mapToRelevantLights,
			size_t tileCount,
			size_t activeTileCount,
			ResourceRegistry &registry			
		);

						
		public: void ExecuteOperationOnResourceReferences( UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(CommandProcessor &context) override;
		
	};
	
	
}
