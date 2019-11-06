#pragma once
#include <d3d12.h>
#include "Shared/PtrTypes.hpp"
#include <list>



namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class Heap;
		struct HeapAllocation;
	}
	
}



namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		class DynamicHeapSet
		{
			private: using t_item = UniquePtr<Heap>;
			
			private: using t_container = std::list<t_item>;

			
			private: size_t estimateBytesPerHeap;

			private: const float allowedEstimateDeviation;

			private: DeviceResources *resources;

			private: t_container heaps;

			private: D3D12_HEAP_FLAGS heapFlags;

					
			
			public: DynamicHeapSet(DeviceResources *resources, size_t initialHeapSizeInBytes, D3D12_HEAP_FLAGS flags);

			
			public: HeapAllocation Allocate(size_t sizeInBytes);
			
		};

		
	}
	
	
}
