#pragma once
#include <list>
#include <vector>
#include "Shared/PtrTypes.hpp"
#include <d3d12.h>
#include <unordered_map>
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
		class ResourceMemory
		{
			using t_item = UniquePtr<RHA::DX12::Heap>;
			
			using t_container = std::vector<t_item>;
			
			struct MemoryInfo
			{				
				size_t heapIndex;
				size_t offsetToAllocation;
				size_t sizeInBytes;			
			};

			private: size_t initialHeapSize;

			private: RHA::DX12::DeviceResources *resources;

			private: D3D12_HEAP_FLAGS heapFlags;
			
			private: std::list<MemoryInfo> freeBlocks;
				
			private: std::unordered_map<D3D12_GPU_VIRTUAL_ADDRESS, MemoryInfo> occupiedBlocks;
			
			private: t_container memory;


			
			public: RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes);

			public: void Deallocate(const RHA::DX12::HeapAllocation &allocation);
			
		};

		
	}

	
}
