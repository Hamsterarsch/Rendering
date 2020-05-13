#pragma once
#include "Commands/RenderCommandCompute.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"


namespace Renderer
{
	class MaintainsInternalRenderResources;
	
	namespace DX12
	{
		class ResourceRegistry;
		class DescriptorMemory;
		
		class CommandInitVolumeTileGrid final : public RenderCommandCompute
		{
			private: HandleWrapper gridOutputBuffer;

			private: HandleWrapper gridDataBuffer;

			private: DxPtr<ID3D12Resource> readbackBuffer;

			private: VolumeTileGrid grid;

			private: DescriptorAllocator descAlloc;

			private: Math::VectorUint3 dispatchSize;

			private: static constexpr unsigned GRID_DATA_INDEX{ 1 }, TABLE_INDEX{ 2 };
			

			
			public: CommandInitVolumeTileGrid
			(
				size_t signatureHandle, 
				size_t psoHandle,
				MaintainsInternalRenderResources &factory, 
				ResourceRegistry &registry, 
				DescriptorMemory &descMem, 
				VolumeTileGrid &&tileGrid, 
				const VolumeTileGridData &gridData
			);					

			public:	void ExecuteOperationOnResourceReferences(UsesReferences *registry,	void(UsesReferences:: *operation)(size_t)) override;
			
			public: void Record(RHA::DX12::CmdList *list, HasQueriableResources &registry) override;
					
			public: void ExecutePostGpuWork() override;
					
			public: void WriteTileData(VolumeTileGrid &output);

			public: size_t GetGridDataBufferHandle() { return gridDataBuffer; }
						
		};

		
	}

	
}
