#pragma once
#include "Resources/AllocationRegistry.hpp"
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


namespace Renderer
{	
	class ResourceFactory
	{
		private: AllocationRegistry allocRegistry;

		private: SharedPtr<class ResourceRegistry> rescRegistry;//actual file io should be done in a seperate library and class
				 


		public: FrameSuballocator MakeAllocatorForNewFrame();

		public: ResourceAllocation FindExistingAllocation(ResourceHandle handle);

		public: void OnNewAllocationMade(ResourceHandle handle);
		
		
	};

	
}
