#pragma once
#include "Shared/PtrTypes.hpp"
#include <map>
#include <vector>


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

		private: std::map<unsigned, std::vector<UniquePtr<RHA::DX12::Heap>>> heaps;

		private: RHA::DX12::DeviceResources *resources;
		
		
		
		public: AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes);


		public: unsigned MakeNewAllocatorID();

		public: void RetireAllocatorID(unsigned ID);
		
		public: RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes, unsigned allocatorID);
		
	};
	
	
}
