#pragma once
#include "Resources/AllocationHeaps.hpp"
#include "FrameSuballocator.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class UploadHeap;
		class Queue;
	}
}


namespace Renderer
{	
	class ResourceFactory
	{
		private: RHA::DX12::Queue *queue;

		private: RHA::DX12::DeviceResources *resources;

		private: AllocationHeaps bufferRescMemory;

		private: AllocationHeaps textureRescMemory;


		
		public: ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue);
		
		public: FrameSuballocator MakeAllocatorForNewFrame();

		public: RHA::DX12::HeapAllocation MakeAllocationForBuffer(size_t sizeInBytes, unsigned allocatorID);
				
	};

	
}
