#pragma once


struct ID3D12Resource;

namespace Renderer
{	
	class FrameSuballocator
	{		
		private: class ResourceFactory *parent;

		private: const unsigned allocatorID;
		
		

		public: FrameSuballocator(class ResourceFactory *parent, unsigned allocatorID);
		
		public: ID3D12Resource MakeBufferWithData(void *data, size_t sizeInBytes);

		public: ID3D12Resource MakeVertexBufferForMesh(struct ResourceHandle handle);
			
	};
	
	
}