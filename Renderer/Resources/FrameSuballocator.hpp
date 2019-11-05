#pragma once
#include "DxPtrTypes.hpp"
#include "Shared/PtrTypes.hpp"

struct ID3D12Resource;

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class UploadHeap;
		class Queue;
		class Fence;
		class CmdAllocator;
		class CmdList;
	}
}


namespace Renderer
{	
	class FrameSuballocator
	{		
		private: class ResourceFactory *parent;

		private: const unsigned allocatorID;

		private: UniquePtr<RHA::DX12::UploadHeap> uploadHeap;

		private: RHA::DX12::Queue *queue;

		private: RHA::DX12::DeviceResources *resources;

		private: UniquePtr<RHA::DX12::Fence> fence;

		private: HANDLE event;

		private: UniquePtr<RHA::DX12::CmdAllocator> allocator;

		private: UniquePtr<RHA::DX12::CmdList> list;
		
		

		public: FrameSuballocator(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue, class ResourceFactory *parent, unsigned allocatorID);
		
		public: DxPtr<ID3D12Resource> MakeBufferWithData(void *data, size_t sizeInBytes);

		public: DxPtr<ID3D12Resource> MakeVertexBufferForMesh(struct ResourceHandle handle);
			
	};
	
	
}