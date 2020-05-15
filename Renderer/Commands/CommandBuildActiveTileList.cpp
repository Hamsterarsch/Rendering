#include "Commands/CommandBuildActiveTileList.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"

namespace Renderer::DX12
{
	CommandBuildActiveTileList::CommandBuildActiveTileList
	(
		const size_t signatureHandle,
		const size_t psoHandle,
		const size_t tileFlagBufferHandle,
		const size_t volumeTileCount,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		RenderCommandCompute{ signatureHandle, psoHandle },
		tileFlagBufferHandle{ tileFlagBufferHandle },
		tileCountBuffer{ &factory, factory.MakeBuffer(&volumeTileCount, sizeof(volumeTileCount)) },
		activeTileListBuffer{ &factory, factory.MakeUavBuffer(nullptr, sizeof(unsigned)*volumeTileCount) },
		counter{ &factory, factory.MakeUavBuffer(nullptr, 4) },
		descAlloc{ descMem.GetDescriptorAllocator(2, 0) },
		groupsToDispatch{ static_cast<size_t>(std::ceil(volumeTileCount / 32.f)) }
	{
		descAlloc.OpenNewTable();
		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(tileFlagBufferHandle),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 1),
			0,
			volumeTileCount,
			sizeof(unsigned)
		);

		descAlloc.CreateUavBufferWithCounter
		(
			registry.GetResource(activeTileListBuffer),
			registry.GetResource(counter),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			volumeTileCount,
			sizeof(unsigned)
		);

	}

	void CommandBuildActiveTileList::ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *const operation)(size_t))
	{
		(registry->*operation)(tileFlagBufferHandle);
		(registry->*operation)(tileCountBuffer);
		(registry->*operation)(activeTileListBuffer);
		(registry->*operation)(counter);
			
	}

	void CommandBuildActiveTileList::Record(RHA::DX12::CmdList *list, HasQueriableResources &registry)
	{
		list->RecordSetComputeSignatureTable(2, descAlloc.GetCurrentTableStartForView());
		list->RecordSetComputeSignatureCbv(1, registry.GetResourceGpuAddress(tileCountBuffer));

		list->RecordBarrierTransition(registry.GetResource(tileFlagBufferHandle), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
		
		list->RecordDispatch(groupsToDispatch, 1, 1);			
		
	}

	
}
