#pragma once
#include <list>
#include <vector>
#include "Shared/PtrTypes.hpp"
#include <d3d12.h>
#include <unordered_map>
#include "DeallocatableGpuMemory.hpp"
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
		class ResourceMemory : public DeallocatableGpuMemory
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
			
			using t_container = std::vector<HeapInfo>;

			
			private: RHA::DX12::DeviceResources *resources;
			
			private: size_t initialHeapSize;

			private: size_t alignment;

			private: D3D12_HEAP_FLAGS heapFlags;
			
			private: std::list<MemoryInfo> freeBlocks;
				
			private: std::unordered_map<ID3D12Heap *, size_t> heapIndexMap;

			private: t_container memory;



			public: ResourceMemory(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes, size_t alignment, D3D12_HEAP_FLAGS heapFlags);

				private: decltype(freeBlocks)::iterator MakeNewFreeBlock(size_t sizeInBytes);

			
			public: virtual RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes) override;

			public: virtual void Deallocate(const RHA::DX12::HeapAllocation &allocation) override;
			
		};

		
	}

	
}
