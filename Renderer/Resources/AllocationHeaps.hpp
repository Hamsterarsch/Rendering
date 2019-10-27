#pragma once


namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		struct HeapAllocation;
	}
	
}


namespace Renderer
{
	class AllocationHeaps
	{
		private: size_t estimateBytesPerHeap;

		
		
		public: AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapEstimate);


		public: RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes, unsigned allocatorID);
		
	};
	
	
}
