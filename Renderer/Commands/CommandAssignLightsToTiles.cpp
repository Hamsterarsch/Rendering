#include "Commands/CommandAssignLightsToTiles.hpp"
#include "ShaderRelevantTypes/Light.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Math/Interface/Types/BoundingBox.hpp"


namespace Renderer::DX12
{
	CommandAssignLightsToTiles::CommandAssignLightsToTiles 
	(		
		const size_t signatureHandle,
		const size_t psoHandle,
		const size_t activeTileListHandle,
		const size_t globalLightListHandle,
		const size_t gridAABBHandle,
		const size_t tileGridDataHandle,
		const size_t lightCount,
		const size_t tileCount,		
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		RenderCommandCompute{ signatureHandle, psoHandle },		
		globalLightListHandle{ globalLightListHandle },
		gridAABBHandle{ gridAABBHandle },
		activeTileListHandle{ activeTileListHandle },
		tileGridDataHandle{ tileGridDataHandle },
		relevantLightIndices{ &factory, factory.MakeUavBuffer(nullptr, lightCount * tileCount * sizeof(unsigned) / 2) },//todo: use the active tile count instead
		offsetsToRelevantLightChunks{ &factory, factory.MakeUavBuffer(nullptr, sizeof(LightIndexListInfo) * tileCount) },
		descAlloc{ descMem.GetDescriptorAllocator(5, 0) },
		dispatchSize{ static_cast<size_t>(std::ceil(tileCount / 32.f)) }
	{
		descAlloc.OpenNewTable();
		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(globalLightListHandle),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 1),
			0,
			lightCount,
			sizeof(Light)
		);

		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(gridAABBHandle),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 2),
			0,
			tileCount,
			sizeof(Math::BoundingBox)
		);

		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(activeTileListHandle),
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
			tileCount * lightCount,
			DXGI_FORMAT_R16_UINT
		);

		descAlloc.CreateUavBuffer
		(
			registry.GetResource(offsetsToRelevantLightChunks),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 2),
			0,
			tileCount,
			sizeof(LightIndexListInfo)	
		);
		
	}


	
	void CommandAssignLightsToTiles::ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *const operation)(size_t))
	{		
		(registry->*operation)(globalLightListHandle);
		(registry->*operation)(gridAABBHandle);
		(registry->*operation)(tileGridDataHandle);
		(registry->*operation)(activeTileListHandle);
		(registry->*operation)(relevantLightIndices);		
		(registry->*operation)(offsetsToRelevantLightChunks);
		
	}


	
	void CommandAssignLightsToTiles::Record(RHA::DX12::CmdList *list, HasQueriableResources &registry)
	{
		list->RecordBarrierTransition(registry.GetResource(gridAABBHandle), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
		list->RecordBarrierTransition(registry.GetResource(activeTileListHandle), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

		list->RecordSetGraphicsSignatureCbv(1, registry.GetResourceGpuAddress(tileGridDataHandle));
		list->RecordSetComputeSignatureTable(2, descAlloc.GetCurrentTableStartForView());

		list->RecordDispatch(dispatchSize, 1, 1);

	}

	
}
