#pragma once


struct ID3D12Resource;

namespace Renderer
{
	class FrameSuballocator
	{

		

		public: ID3D12Resource MakeBufferWithData(void *data, size_t sizeInBytes);

		public: ID3D12Resource MakeVertexBufferForMesh(struct ResourceHandle handle);
		
		
	};

	

	
}