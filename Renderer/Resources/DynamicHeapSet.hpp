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
	class DynamicHeapSet
	{
		private: using t_item = UniquePtr<RHA::DX12::Heap>;
		
		private: using t_container = std::list<t_item>;

		
		private: size_t estimateBytesPerHeap;

		private: const float allowedEstimateDeviation;

		private: RHA::DX12::DeviceResources *resources;

		private: t_container heaps;

		private: D3D12_HEAP_FLAGS heapFlags;

				
		
		public: DynamicHeapSet(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes, D3D12_HEAP_FLAGS flags);

		
		public: RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes);
		
	};
	
	
}
