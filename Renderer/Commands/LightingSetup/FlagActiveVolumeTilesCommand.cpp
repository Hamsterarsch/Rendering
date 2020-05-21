#include "Commands/LightingSetup/FlagActiveVolumeTilesCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/RenderMeshCommand.hpp"
#include "Commands/LightingSetup/GenerateActiveTileListCommand.hpp"


namespace Renderer::DX12::Commands
{
	size_t FlagActiveVolumeTilesCommand::signature{ 0 };

	size_t FlagActiveVolumeTilesCommand::pso{ 0 };
	
	
	FlagActiveVolumeTilesCommand::FlagActiveVolumeTilesCommand
	(
		GenerateActiveTileListCommand &pass,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry, 
		DescriptorMemory &descMem
	) :
		RenderCommandGraphics{ signature, pso },
		pass{ &pass },
		descAlloc{ descMem.GetDescriptorAllocator(2,0) }
	{
				
		descAlloc.OpenNewTable();
		descAlloc.CreateUavBuffer
		(
			registry.GetResource(pass.tileFlags),
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			pass.lightGridTileCount,
			sizeof(unsigned)
		);

		auto *resource{ registry.GetResource(pass.lightGridData) };
		auto desc{ resource->GetDesc() };
					
		descAlloc.CreateCbv
		(
			resource,
			registry.GetSignatureCbvOffset(GetSignatureHandle(), 1),
			desc.Width
		);
		
	}

	

	void FlagActiveVolumeTilesCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *const operation)(size_t))
	{
		RenderCommandGraphics::ExecuteOperationOnResourceReferences(registry, operation);
		
		for(auto &&cmd : meshCommands)
		{
			cmd.ExecuteOperationOnResourceReferences(registry, operation);
		}			
		
	}


	
	void FlagActiveVolumeTilesCommand::Execute(DX12CommandProcessor &context)
	{
		RenderCommandGraphics::Execute(context);
		
		auto &list{ context.GetList() };
		auto &registry{ context.GetRegistry() };
		
		list.RecordSetGraphicsSignatureTable(2, descAlloc.GetCurrentTableStartForView());

		for(auto &&cmd : meshCommands)
		{
			cmd.Execute(context);
		}			
									
	}

	

	void FlagActiveVolumeTilesCommand::AddRenderMeshCommand(const RenderMeshArguments &command)
	{
		meshCommands.emplace_back(signature, pso, command);
		
	}

	
}
