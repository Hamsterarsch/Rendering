#include "Commands/LightingSetup/AssignLightsToTilesCommand.hpp"
#include "ShaderRelevantTypes/Light.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Math/Interface/Types/BoundingBox.hpp"


namespace Renderer::DX12::Commands
{
	size_t AssignLightsToTilesCommand::signature{ 0 };

	size_t AssignLightsToTilesCommand::pso{ 0 };
	
	
	AssignLightsToTilesCommand::AssignLightsToTilesCommand
	(		
		const size_t activeTileList,
		const size_t globalLightList,
		const size_t tileGridBoundingBoxes,			
		const size_t tileGridData,
		const size_t lightCount,
		const size_t tileCount,
		const size_t activeTileCount,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		RenderCommandCompute{ signature, pso },		
		globalLightList{ globalLightList },
		tileGridBoundingBoxes{ tileGridBoundingBoxes },
		activeTileList{ activeTileList },
		tileGridData{ tileGridData },
		relevantLightIndices{ &factory, factory.MakeUavBuffer(nullptr, lightCount * activeTileCount * sizeof(unsigned) / 2) },
		mappingToRelevantLights{ &factory, factory.MakeUavBuffer(nullptr, sizeof(LightIndexListInfo) * tileCount) },
		descAlloc{ descMem.GetDescriptorAllocator(5, 0) },
		dispatchSize{ static_cast<size_t>(std::ceil(tileCount / 32.f)) }
	{
		descAlloc.OpenNewTable();
		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(globalLightList),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 1),
			0,
			lightCount,
			sizeof(Light)
		);

		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(tileGridBoundingBoxes),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 2),
			0,
			tileCount,
			sizeof(Math::BoundingBox)
		);

		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(activeTileList),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 3),
			0,
			tileCount+1,
			sizeof(unsigned)
		);


		descAlloc.CreateUavBufferFormatted
		(
			registry.GetResource(relevantLightIndices),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			activeTileCount * lightCount,
			DXGI_FORMAT_R16_UINT
		);

		descAlloc.CreateUavBuffer
		(
			registry.GetResource(mappingToRelevantLights),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 2),
			0,
			tileCount,
			sizeof(LightIndexListInfo)	
		);
		
	}


	
	void AssignLightsToTilesCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *const operation)(size_t))
	{
		RenderCommandCompute::ExecuteOperationOnResourceReferences(registry, operation);
		
		(registry.*operation)(globalLightList);
		(registry.*operation)(tileGridBoundingBoxes);
		(registry.*operation)(tileGridData);
		(registry.*operation)(activeTileList);
		(registry.*operation)(relevantLightIndices);		
		(registry.*operation)(mappingToRelevantLights);
		
	}


	
	void AssignLightsToTilesCommand::Execute(CommandProcessor &context)
	{
		RenderCommandCompute::Execute(context);

		auto &list{ context.GetList() };
		auto &registry{ context.GetRegistry() };
		
		list.RecordBarrierTransition(registry.GetResource(tileGridBoundingBoxes), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);		

		list.RecordSetComputeSignatureCbv(1, registry.GetResourceGpuAddress(tileGridData));
		list.RecordSetComputeSignatureTable(2, descAlloc.GetCurrentTableStartForView());

		list.RecordDispatch(dispatchSize, 1, 1);

		list.RecordBarrierTransition(registry.GetResource(relevantLightIndices), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		list.RecordBarrierTransition(registry.GetResource(mappingToRelevantLights), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	}

	
}
