#pragma once
#include "Resources/AllocationRegistry.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Shared/PtrTypes.hpp"
#include "FrameSuballocator.hpp"



/*
 enum class ResourceTypes : char
{
	Mesh,
	PSO,
	Texture
};

struct ResourceHandle
{
	ResourceTypes type;
	size_t serial;	
};

class ResourceMngr;

class FrameSubmngr
{
	const size_t frameNumber;
	ResourceMngr *parent;

	public: struct Buffer LoadDataToBuffer(void *data, size_t sizeInBytes);

	public: struct Buffer LoadMeshVerticesToBuffer(ResourceHandle handle)
		
		struct allocation RequestResourceDataFromParent(ResourceHandle handle);

		void AllocateResource(ResourceHandle ResourceHandle);
	
};

class ResourceMngr
{
	private: class ResourceRegistry *registry;//shared ptr

	private: class loadedResources *loaded;
	
	
	public: FrameSubmngr GetSuballocatorForFrame();

	public: allocation FindResourceData(ResourceHandle handle);

		private: void AddResourceToLoaded(ResourceHandle handle);
	
	
};
 */

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

		private: UniquePtr<RHA::DX12::UploadHeap> uploadHeap;

		private: RHA::DX12::Queue *queue;


		
		public: ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue);
		
		public: FrameSuballocator MakeAllocatorForNewFrame();

		public: ResourceAllocation FindExistingAllocation(ResourceHandle handle);

		public: void OnNewAllocationMade(ResourceHandle handle);
		
		
	};

	
}
