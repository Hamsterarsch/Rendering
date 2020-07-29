#include "Commands/LightingSetup/BuildActiveTileListCommand.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/LightingSetup/GenerateActiveTileListCommand.hpp"

namespace Renderer::DX12::Commands
{
	size_t BuildActiveTileListCommand::signature{ 0 };

	size_t BuildActiveTileListCommand::pso{ 0 };
	
	
	BuildActiveTileListCommand::BuildActiveTileListCommand
	(
		GenerateActiveTileListCommand &pass,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		RenderCommandCompute{ signature, pso },
		pass{ &pass },		
		tileCountBuffer{ &factory, factory.MakeBufferWithData(&pass.lightGridTileCount, sizeof(pass.lightGridTileCount)) },		
		descAlloc{ descMem.GetDescriptorAllocator(2, 0) },
		groupsToDispatch{ static_cast<size_t>(std::ceil(pass.lightGridTileCount / 32.f)) }
	{		
		
		descAlloc.OpenNewTable();
		descAlloc.CreateSrvBuffer
		(
			registry.GetResource(pass.tileFlags),
			registry.GetSignatureSrvOffset(GetSignatureHandle(), 1),
			0,
			pass.lightGridTileCount,
			sizeof(unsigned)
		);

		descAlloc.CreateUavBuffer
		(
			registry.GetResource(pass.activeTileList),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			pass.lightGridTileCount+1,
			sizeof(unsigned)
		);

	}

	void BuildActiveTileListCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *const operation)(size_t))
	{
		RenderCommandCompute::ExecuteOperationOnResourceReferences(registry, operation);
		
		(registry.*operation)(tileCountBuffer);
			
	}

	void BuildActiveTileListCommand::Execute(DX12CommandProcessor &context)
	{
		RenderCommandCompute::Execute(context);

		auto &list{ context.GetList() };
				
		list.RecordSetComputeSignatureTable(2, descAlloc.GetCurrentTableStartForView());
		list.RecordSetComputeSignatureCbv(1, context.GetRegistry().GetResourceGpuAddress(tileCountBuffer));

		list.RecordDispatch(groupsToDispatch, 1, 1);			
		
	}

	
}
