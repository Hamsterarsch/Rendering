#pragma once
#include "Resources/ResourceAllocation.hpp"


struct ID3D12Resource;

namespace Renderer
{	
	class FrameSuballocator
	{
		private: ResourceAllocation allocationRange;

		private: class ResourceFactory *parent;
		
		

		public: FrameSuballocator(const ResourceAllocation &allocationRange, class ResourceFactory *parent);
		
		public: ID3D12Resource MakeBufferWithData(void *data, size_t sizeInBytes);

		public: ID3D12Resource MakeVertexBufferForMesh(struct ResourceHandle handle);
			
	};
	
	
}