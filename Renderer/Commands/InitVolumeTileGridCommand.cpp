#include "Commands/InitVolumeTileGridCommand.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/ResourceRegistry.hpp"


namespace Renderer::DX12::Commands
{	
	InitVolumeTileGridCommand::InitVolumeTileGridCommand
	(
		const size_t signatureHandle, 
		const size_t psoHandle,
		VolumeTileGrid &&tileGrid,
		const VolumeTileGridData &gridData,
		MaintainsInternalRenderResources &factory,
		ResourceRegistry &registry,
		DescriptorMemory &descMem
	)	:
		RenderCommandCompute{signatureHandle, psoHandle},
		grid{ std::move(tileGrid) },
		descAlloc{ descMem.GetDescriptorAllocator(1,0) }
	{
		gridOutputBuffer = HandleWrapper{ &factory, factory.MakeUaBuffer(grid.GetData(), grid.SizeInBytes()) };
		gridDataBuffer = HandleWrapper{ &factory, factory.MakeBuffer(&gridData, sizeof std::remove_reference_t<decltype(gridData)>) };
		readbackBuffer = factory.MakeReadbackBuffer(grid.SizeInBytes());
		
		descAlloc.OpenNewTable();
		descAlloc.CreateUavBuffer
		(
			registry.GetResource(gridOutputBuffer), 
			registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
			0,
			grid.GetTileCount(),
			grid.GetTileStride()
		);
						
		dispatchSize.x = std::ceil(gridData.outGridDimensions.x / 4.f);
		dispatchSize.y = std::ceil(gridData.outGridDimensions.y / 4.f);
		dispatchSize.z = std::ceil(gridData.outGridDimensions.z / 4.f);
		
	}
	
	
	
	void InitVolumeTileGridCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *const operation)(size_t))
	{
		RenderCommandCompute::ExecuteOperationOnResourceReferences(registry, operation);
		
		(registry.*operation)(gridOutputBuffer);
		(registry.*operation)(gridDataBuffer);
		
	}

	void InitVolumeTileGridCommand::Execute(DX12CommandProcessor &context)
	{
		RenderCommandCompute::Execute(context);

		{
			auto &list{ context.GetList() };
			auto &registry{ context.GetRegistry() };
			
			list.RecordSetComputeSignatureCbv(GRID_DATA_INDEX, registry.GetResourceGpuAddress(gridDataBuffer));								
			list.RecordSetComputeSignatureTable(TABLE_INDEX, descAlloc.GetCurrentTableStartForView());
	
			list.RecordDispatch(dispatchSize.x, dispatchSize.y, dispatchSize.z);
							
			list.RecordBarrierTransition(registry.GetResource(gridOutputBuffer), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);								
			list.RecordCopyResource(readbackBuffer.Get(), registry.GetResource(gridOutputBuffer));
		}
		context.SubmitAndWaitForGpuWork();

		void *readbackData;
		auto mapResult{ readbackBuffer->Map(0, nullptr, &readbackData) };
	
		memcpy(grid.GetData(), readbackData, grid.SizeInBytes());
	
		D3D12_RANGE range{0,0};
		readbackBuffer->Unmap(0, &range);
		
	}

	
	
	void InitVolumeTileGridCommand::WriteTileData(VolumeTileGrid &output)
	{
		output = std::move(grid);
		
	}

	
}