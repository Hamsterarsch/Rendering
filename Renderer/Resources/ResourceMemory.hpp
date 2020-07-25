#pragma once
#include <list>
#include <vector>
#include "Shared/PtrTypes.hpp"
#include <d3d12.h>
#include <unordered_map>
#include "DX12/DeallocatableGpuMemory.hpp"
#include "DX12/HeapAllocation.hpp"


namespace RHA
{
	namespace DX12
	{
		class Heap;
		class DeviceResources;
	}
}

namespace Renderer
{
	namespace DX12
	{
		class ResourceMemory : public RHA::DX12::DeallocatableGpuMemory
		{			
			struct MemoryInfo
			{				
				size_t heapIndex;
				size_t offsetToAllocation;
				size_t sizeInBytes;			
			};

			struct HeapInfo
			{
				UniquePtr<RHA::DX12::Heap>	heap;
				std::unordered_map<size_t, MemoryInfo> occupiedBlocks;
			};
			
			
			private: RHA::DX12::DeviceResources *resources;
			
			private: size_t initialHeapSizeInBytes;

			private: size_t alignment;

			private: D3D12_HEAP_FLAGS heapFlags;

			private: D3D12_HEAP_TYPE type;
			
			private: std::list<MemoryInfo> freeBlocks;
				
			private: std::unordered_map<ID3D12Heap *, size_t> heapIndexMap;

			private: std::vector<HeapInfo> memory;
					 
			private: using t_freeBlocksItr = decltype(freeBlocks)::iterator;
			


			public: ResourceMemory
			(
				RHA::DX12::DeviceResources *resources,
				size_t initialHeapSizeInBytes,
				size_t alignment,
				D3D12_HEAP_FLAGS heapFlags,
				D3D12_HEAP_TYPE type = D3D12_HEAP_TYPE_DEFAULT
			);

				private: decltype(freeBlocks)::iterator MakeNewFreeBlock(size_t sizeInBytes);

						 			
			public: virtual RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes) override;

				private: t_freeBlocksItr FindFreeBlock(size_t sizeInBytes);

				private: size_t GetBlockSizeForNewAllocation(size_t allocationSizeInBytes) const;

				private: RHA::DX12::HeapAllocation AllocateFromBlock(const t_freeBlocksItr &block, size_t allocationSizeInBytes);

					private: size_t GetAlignedAllocationSizeForBlock(const t_freeBlocksItr &block, size_t allocationSizeInBytes);

				private: void RegisterAllocationAsOccupiedBlock(size_t targetHeapIndex, const RHA::DX12::HeapAllocation &allocation);

				private: void AdjustFreeBlockDataAfterAllocation(t_freeBlocksItr &block, const RHA::DX12::HeapAllocation &allocation);

			
			public: virtual void Deallocate(const RHA::DX12::HeapAllocation &allocation) override;
			
		};

		
	}

	
}
