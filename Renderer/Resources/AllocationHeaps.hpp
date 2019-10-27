#pragma once


namespace RHA{ namespace DX12{ class DeviceResources; }}


namespace Renderer
{
	class AllocationHeaps
	{
		private: size_t estimateBytesPerHeap;

		
		
		public: AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapEstimate);
		
	};
	
	
}
