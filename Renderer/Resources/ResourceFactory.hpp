#pragma once
#include "DxPtrTypes.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/ResourceMemory.hpp"

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
	namespace DX12
	{
		using namespace RHA::DX12;
		
		class ResourceFactory
		{		
			private: UniquePtr<UploadHeap> uploadBuffer;

			private: Queue *queue;

			private: DeviceResources *resources;

			private: UniquePtr<Fence> fence;

			private: HANDLE event;

			private: UniquePtr<CmdAllocator> allocator;

			private: UniquePtr<CmdList> list;

			private: D3D12_GPU_VIRTUAL_ADDRESS uploadAddress;


			
			protected: UniquePtr<AllocatableGpuMemory> memory;

			

			public: ResourceFactory(DeviceResources *resources, Queue *queue, UniquePtr<AllocatableGpuMemory> &&memory);

			public: ResourceFactory(ResourceFactory &&other) noexcept = default;

			public: ResourceFactory(ResourceFactory &other) = delete;

			public: ResourceFactory &operator=(ResourceFactory &&other) noexcept = default;

			public: ResourceFactory &operator=(ResourceFactory &other) = delete;

			public: virtual ~ResourceFactory() noexcept;

											
			public: ResourceAllocation MakeBufferWithData(const void *data, size_t sizeInBytes, D3D12_RESOURCE_STATES desiredState, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);
								
				private: void CopyDataToUploadBuffer(const void *data, size_t sizeInBytes);

					private: bool UploadBufferCanNotFitAllocation(size_t allocationSizeInBytes) const;
			
				private: ResourceAllocation MakePlacedBufferResource(size_t sizeInBytes, D3D12_RESOURCE_FLAGS resourceFlags, D3D12_RESOURCE_STATES resourceState);

					private: static D3D12_RESOURCE_DESC MakeBufferDesc(size_t sizeInBytes, D3D12_RESOURCE_FLAGS flags);

				private: static void CheckGpuResourceCreation(HRESULT result);

				private: void ClearCmdList();

				private: void SubmitListAndFenceSynchronization(CmdList *list);

			public: DxPtr<ID3D12Resource> MakeCommittedBuffer
			(
				size_t sizeInBytes,
				D3D12_RESOURCE_STATES desiredState,
				D3D12_HEAP_TYPE heapType, 
				D3D12_HEAP_FLAGS heapFlags,
				D3D12_RESOURCE_FLAGS bufferFlags = D3D12_RESOURCE_FLAG_NONE
			);


			public: void Deallocate(ResourceAllocation &allocation, ResourceTypes type);

			protected: virtual void DeallocateInternal(ResourceAllocation &allocation, ResourceTypes type) {};
			

						
		};

		
	}

	
}
