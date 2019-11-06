#pragma once
#include "Resources/AllocationRegistry.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/AllocationHeaps.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Shared/PtrTypes.hpp"
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
		private: AllocationRegistry allocRegistry;

		private: ResourceRegistry rescRegistry;//actual file io should be done in a seperate library and class

		private: RHA::DX12::Queue *queue;

		private: RHA::DX12::DeviceResources *resources;

		private: AllocationHeaps bufferRescMemory;

		private: AllocationHeaps textureRescMemory;
				 		
		public: ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue);
		
		public: FrameSuballocator MakeAllocatorForNewFrame();

		public: ResourceAllocation FindExistingAllocation(ResourceHandle handle);

		public: RHA::DX12::HeapAllocation MakeRawAllocationForBuffer(size_t sizeInBytes, unsigned allocatorID);

		
		
		public: void OnNewAllocationMade(ResourceHandle handle);
		
		
	};

	
}
