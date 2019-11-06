#pragma once
#include "DxPtrTypes.hpp"
#include "Shared/PtrTypes.hpp"
#include "DynamicHeapSet.hpp"

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
	class ResourceFactory
	{		

		private: UniquePtr<RHA::DX12::UploadHeap> uploadBuffer;

		private: RHA::DX12::Queue *queue;

		private: RHA::DX12::DeviceResources *resources;

		private: UniquePtr<RHA::DX12::Fence> fence;

		private: HANDLE event;

		private: UniquePtr<RHA::DX12::CmdAllocator> allocator;

		private: UniquePtr<RHA::DX12::CmdList> list;

		private: D3D12_GPU_VIRTUAL_ADDRESS uploadAddress;
		
		private: DynamicHeapSet bufferHeaps;
		

		public: ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue);
		
		public: DxPtr<ID3D12Resource> MakeBufferWithData(const void *data, size_t sizeInBytes);

			private: void CopyDataToUploadBuffer(const void *data, size_t sizeInBytes);

				private: bool UploadBufferCanNotFitAllocation(size_t allocationSizeInBytes) const;

			private: static D3D12_RESOURCE_DESC MakeBufferDesc(size_t sizeInBytes);

			private: static void CheckGpuResourceCreation(HRESULT result);

			private: DxPtr<ID3D12GraphicsCommandList> GetFreshCmdList();

			private: void SubmitListAndFenceSynchronization(RHA::DX12::CmdList *list);
					
	};

	
}
