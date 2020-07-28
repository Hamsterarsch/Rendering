#include "Commands/LightingSetup/GenerateActiveTileListCommand.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"



namespace Renderer::DX12::Commands
{
	GenerateActiveTileListCommand::GenerateActiveTileListCommand
	(
		const size_t lightCount,
		const size_t lightGridTileCount,
		const size_t lightGridBoundingBoxes,
		const size_t lightsList,
		const size_t lightGridData,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		lightCount{ lightCount },
		lightGridTileCount{ lightGridTileCount },
		lightGridBoundingBoxes{ lightGridBoundingBoxes },
		lightsList{ lightsList },
		lightGridData{ lightGridData },
		tileFlags{ &factory, factory.MakeUaBuffer(nullptr, lightGridTileCount * sizeof(unsigned)) },
		activeTileList{ &factory, factory.MakeUaBuffer(nullptr, sizeof(unsigned)*(lightGridTileCount+1)) },
		activeTileCount{ 0 },
		flagTilesCommand{ *this, factory, registry, descMem },
		buildActiveTileListCommand{ *this, factory, registry, descMem },
		activeTileCountReadback{ factory.MakeReadbackBuffer(sizeof(unsigned)) }
	{			
	}


	
	void GenerateActiveTileListCommand::Execute(DX12CommandProcessor &context)
	{				
		auto &list{ context.GetList() };
		auto &registry{ context.GetRegistry() };
		
		flagTilesCommand.Execute(context);

		list.RecordBarrierTransition(registry.GetResource(tileFlags), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);		
		
		buildActiveTileListCommand.Execute(context);

		list.RecordBarrierTransition(registry.GetResource(activeTileList), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
		
		list.RecordCopyBufferRegion(activeTileCountReadback.Get(), 0, registry.GetResource(activeTileList), 0, sizeof(unsigned));

		list.RecordBarrierTransition(registry.GetResource(activeTileList), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

		context.SubmitAndWaitForGpuWork();

		D3D12_RANGE range{ 0, sizeof(unsigned) };
		void *data;

		activeTileCountReadback->Map(0, &range, &data);
		memcpy(&activeTileCount, data, sizeof activeTileCount);
		--activeTileCount;
		
		range.End = 0;
		activeTileCountReadback->Unmap(0, &range);
		
	}


	
	void GenerateActiveTileListCommand::AddRenderMeshCommand(const RenderMeshArguments &command)
	{
		flagTilesCommand.AddRenderMeshCommand(command);
		
	}

	
}
