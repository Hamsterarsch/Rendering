#include "Commands/CommandFlagActiveVolumeTiles.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/RenderMeshCommand.hpp"


namespace Renderer::DX12
{	
	CommandFlagActiveVolumeTiles::CommandFlagActiveVolumeTiles
	(
		const size_t signatureHandle,
		const size_t psoHandle,
		const size_t volumeTileGridDataBufferHandle,
		const size_t volumeTileCount,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry, 
		DescriptorMemory &descMem
	) :
		RenderCommandGraphics{ signatureHandle, psoHandle },
		tileFlagsSizeInBytes{ volumeTileCount * sizeof(unsigned) },						
		flagBufferHandle{ &factory, factory.MakeUavBuffer(nullptr, tileFlagsSizeInBytes) },
		volumeTileDataBuffer{ volumeTileGridDataBufferHandle },
		descAlloc{ descMem.GetDescriptorAllocator(2,0) }
	{
		descAlloc.OpenNewTable();
		descAlloc.CreateUavBuffer
		(
			registry.GetResource(flagBufferHandle),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			volumeTileCount,
			sizeof(unsigned)
		);

		auto *resource{ registry.GetResource(volumeTileDataBuffer) };
		auto desc{ resource->GetDesc() };
					
		descAlloc.CreateCbv
		(
			resource,
			registry.GetSignatureCbvOffset(GetSignatureHandle(), 1),
			desc.Width
		);
		
	}

	

	void CommandFlagActiveVolumeTiles::ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *const operation)(size_t))
	{
		(registry->*operation)(volumeTileDataBuffer);
		(registry->*operation)(flagBufferHandle);

		for(auto &&cmd : meshCommands)
		{
			cmd.ExecuteOperationOnResourceReferences(registry, operation);
		}			
		
	}


	
	void CommandFlagActiveVolumeTiles::Record(RHA::DX12::CmdList *list, HasQueriableResources &registry)
	{										
		list->RecordSetGraphicsSignatureTable(2, descAlloc.GetCurrentTableStartForView());

		for(auto &&cmd : meshCommands)
		{
			cmd.Record(list, registry);
		}			
									
	}

	

	void CommandFlagActiveVolumeTiles::AddRenderMeshCommand(const RenderMeshCommand &command)
	{
		meshCommands.push_back(command);
		
	}

	
}
