#pragma once
#include "Resources/ResourceAllocation.hpp"


struct ID3D12Resource;

namespace Renderer
{	
	class FrameSuballocator
	{
		private: ResourceAllocation allocationRange;

		private: class ResourceFactory *parent;

		private: const unsigned allocatorID;
		
		

		public: FrameSuballocator(const ResourceAllocation &allocationRange, class ResourceFactory *parent, unsigned allocatorID);
		
		public: ID3D12Resource MakeBufferWithData(void *data, size_t sizeInBytes);

		public: ID3D12Resource MakeVertexBufferForMesh(struct ResourceHandle handle);
			
	};
	
	
}