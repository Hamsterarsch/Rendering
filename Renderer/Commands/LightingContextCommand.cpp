#include "Commands/LightingContextCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "ShaderRelevantTypes/Light.hpp"

namespace Renderer::DX12::Commands
{
	LightingContextCommand::LightingContextCommand
	(
		DescriptorMemory &descriptorMemory,
		HandleWrapper &&buffer,
		const size_t lightingSignature,
		const size_t tileGridData,
		const size_t lightsList,
		const size_t relevantLightsList,
		const size_t mapToRelevantLights,
		const size_t tileCount,
		const size_t activeTileCount,
		ResourceRegistry &registry
	)	:
		gbCommand{ descriptorMemory, std::move(buffer) },
		lightingDescriptors{ descriptorMemory.GetDescriptorAllocator(4, 0) },
		tileGridData{ tileGridData },
		lightsList{ lightsList },
		relevantLightsList{ relevantLightsList },
		mapToRelevantLights{ mapToRelevantLights }
	{
		
		lightingDescriptors.OpenNewTable();
		lightingDescriptors.CreateCbv
		(
			registry.GetResource(tileGridData),
			registry.GetSignatureCbvOffset(lightingSignature, 1),
			registry.GetResource(tileGridData)->GetDesc().Width
		);

		lightingDescriptors.CreateSrvBuffer
		(
			registry.GetResource(lightsList),
			registry.GetSignatureSrvOffset(lightingSignature, 1),
			0,
			registry.GetLightCount(),
			registry.GetLigthsDataSizeInBytes() / registry.GetLightCount()
		);

		lightingDescriptors.CreateSrvBufferFormatted
		(
			registry.GetResource(relevantLightsList),
			registry.GetSignatureSrvOffset(lightingSignature, 2),
			0,
			registry.GetLightCount() * activeTileCount,
			DXGI_FORMAT_R16_UINT			
		);

		lightingDescriptors.CreateSrvBuffer
		(
			registry.GetResource(mapToRelevantLights),
			registry.GetSignatureSrvOffset(lightingSignature, 3),
			0,
			tileCount,			
			sizeof LightIndexListInfo
		);		
		
	}


	
	void LightingContextCommand::ExecuteOperationOnResourceReferences( UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		gbCommand.ExecuteOperationOnResourceReferences(registry, operation);
				
		(registry.*operation)(tileGridData);
		(registry.*operation)(lightsList);
		(registry.*operation)(relevantLightsList);
		(registry.*operation)(mapToRelevantLights);
		
	}


	
	void LightingContextCommand::Execute(DX12CommandProcessor &context)
	{
		gbCommand.Execute(context);

		if(context.DoesContextEventMatch(CommandContextEvents::GraphicsSignatureChanged))
		{
			context.GetList().RecordSetGraphicsSignatureTable(2, lightingDescriptors.GetCurrentTableStartForView());
		}
		
	}

	
}
