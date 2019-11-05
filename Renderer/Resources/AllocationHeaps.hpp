#pragma once
#include <d3d12.h>
#include "Shared/PtrTypes.hpp"
#include <map>
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
	class AllocationHeaps
	{
		private: size_t estimateBytesPerHeap;

		private: const float allowedEstimateDeviation;

		private: unsigned newestDeclaredAllocatorID;

		private: std::map<unsigned, std::list<UniquePtr<RHA::DX12::Heap>>> heaps;

		private: RHA::DX12::DeviceResources *resources;

		private: D3D12_HEAP_FLAGS heapFlags;
		
		
		
		public: AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes, D3D12_HEAP_FLAGS flags);


		public: unsigned MakeNewAllocatorID();

		public: void RetireAllocatorID(unsigned ID);
		
		public: RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes, unsigned allocatorID);
		
	};
	
	
}
