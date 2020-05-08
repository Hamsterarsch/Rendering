#pragma once
#include "Commands/RenderCommandCompute.hpp"


#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"

#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/ResourceRegistry.hpp"

namespace Renderer
{
	
	namespace DX12
	{
		class CommandInitVolumeTileGrid final : public RenderCommandCompute
		{
			private: HandleWrapper gridOutputBuffer;

			private: HandleWrapper gridDataBuffer;

			private: DxPtr<ID3D12Resource> readbackBuffer;

			private: VolumeTileGrid grid;

			private: DescriptorAllocator descAlloc;

			private: Math::VectorUint3 dispatchSize;

			private: static constexpr unsigned GRID_DATA_INDEX{ 1 }, TABLE_INDEX{ 2 };
			
			
			public: CommandInitVolumeTileGrid(size_t signatureHandle, size_t psoHandle, MaintainsInternalRenderResources &r, ResourceRegistry &registry, DescriptorMemory &descMem, VolumeTileGrid &&tileGrid, const VolumeTileGridData &gridData)
				:
				RenderCommandCompute{signatureHandle, psoHandle},
				grid{ std::move(tileGrid) },
				descAlloc{ descMem.GetDescriptorAllocator(1,0) }
			{
				gridOutputBuffer = HandleWrapper{ &r, r.MakeUavBuffer(grid.GetData(), grid.SizeInBytes()) };
				gridDataBuffer = HandleWrapper{ &r, r.MakeBuffer(&gridData, sizeof std::remove_reference_t<decltype(gridData)>) };
				readbackBuffer = r.MakeReadbackBuffer(grid.SizeInBytes());
				
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


			public:	void ExecuteOperationOnResourceReferences(UsesReferences* registry,	void( UsesReferences::* operation)(size_t)) override
			{
				(registry->*operation)(gridOutputBuffer);
				(registry->*operation)(gridDataBuffer);
				
			}
			
			public: void Record(RHA::DX12::CmdList* list, HasQueriableResources& registry) override
			{
				list->RecordSetComputeSignatureCbv(GRID_DATA_INDEX, registry.GetResourceGpuAddress(gridDataBuffer));								
				list->RecordSetComputeSignatureTable(TABLE_INDEX, descAlloc.GetCurrentTableStartForView());

				list->RecordDispatch(dispatchSize.x, dispatchSize.y, dispatchSize.z);
								
				list->RecordBarrierTransition(registry.GetResource(gridOutputBuffer), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);								
				list->RecordCopyResource(readbackBuffer.Get(), registry.GetResource(gridOutputBuffer));
				
			}

			public: void ExecutePostGpuWork() override
			{
				void *readbackData;
				auto mapResult{ readbackBuffer->Map(0, nullptr, &readbackData) };

				memcpy(grid.GetData(), readbackData, grid.SizeInBytes());

				D3D12_RANGE range{0,0};
				readbackBuffer->Unmap(0, &range);				
			}

			public: void WriteTileData(VolumeTileGrid &output) { output = std::move(grid); }
			

			
		};

		
	}

	
}
