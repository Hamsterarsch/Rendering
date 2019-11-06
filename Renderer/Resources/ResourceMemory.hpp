#pragma once
#include "Resources/DynamicHeap.hpp"
#include "ResourceFactory.hpp"


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
	class ResourceMemory
	{
		private: RHA::DX12::Queue *queue;

		private: RHA::DX12::DeviceResources *resources;

		private: DynamicHeap bufferRescMemory;

		private: DynamicHeap textureRescMemory;


		
		public: ResourceMemory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue);
		
		public: ResourceFactory MakeAllocatorForNewFrame();

		public: RHA::DX12::HeapAllocation MakeAllocationForBuffer(size_t sizeInBytes, unsigned allocatorID);
				
	};

	
}
