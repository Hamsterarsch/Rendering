#pragma once
#include "DxPtrTypes.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/ResourceMemory.hpp"


struct ID3D12Resource;
namespace RHA::DX12
{
	class DeviceResources;
	class UploadHeap;
	class Queue;
	class Fence;
	class CmdAllocator;
	class CmdList;
}

namespace Renderer
{
	class RendererFacade;
	
	struct DataSource;
}


namespace Renderer::DX12
{
	class ResourceRegistry;
	using namespace RHA::DX12;

	
	class ResourceFactory
	{					
		private: DeviceResources *resources;

		private: RendererFacade *renderer;
		
		private: ResourceRegistry *registry;
				 		
		private: D3D12_CLEAR_VALUE depthTextureClearValue;

		private: D3D12_CLEAR_VALUE *clearValue;

				 
		
		protected: UniquePtr<AllocatableGpuMemory> bufferMemory;

		protected: UniquePtr<AllocatableGpuMemory> textureMemory;

		protected: UniquePtr<AllocatableGpuMemory> depthTextureMemory;

		protected: UniquePtr<AllocatableGpuMemory> bufferReadbackMemory;

		protected: UniquePtr<AllocatableGpuMemory> uploadMemory;
		
				

		public: ResourceFactory
		(
			DeviceResources *resources,
			RendererFacade &renderer,
			ResourceRegistry &registry,
			UniquePtr<AllocatableGpuMemory> &&bufferMemory,
			UniquePtr<AllocatableGpuMemory> &&textureMemory,
			UniquePtr<AllocatableGpuMemory> &&depthTextureMemory,
			UniquePtr<AllocatableGpuMemory> &&bufferReadbackMemory,
			UniquePtr<AllocatableGpuMemory> &&uploadMemory
		);

		public: ResourceFactory(ResourceFactory &&other) noexcept = default;

		public: ResourceFactory(ResourceFactory &other) = delete;

		public: ResourceFactory &operator=(ResourceFactory &&other) noexcept = default;

		public: ResourceFactory &operator=(ResourceFactory &other) = delete;

		public: virtual ~ResourceFactory() noexcept;

										
		public: ResourceHandle::t_hash MakeBufferWithData(const DataSource *dataSources, unsigned char numDataSources, size_t resourceSizeInBytes, D3D12_RESOURCE_STATES desiredState, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

			private: ResourceHandle::t_hash MakeBufferWithDataInternal
			(
				AllocatableGpuMemory &memorySource,				
				ResourceTypes allocationType,				
				const DataSource *dataSources,
				unsigned char numDataSources,
				size_t resourceSizeInBytes,
				D3D12_RESOURCE_STATES desiredState,
				D3D12_RESOURCE_FLAGS flags
			);

				private: ResourceHandle::t_hash MakePlacedBufferResource
				(
					AllocatableGpuMemory &memorySource,
					ResourceTypes allocationType,
					size_t sizeInBytes,
					D3D12_RESOURCE_FLAGS resourceFlags,
					D3D12_RESOURCE_STATES resourceState
				);

					private: static D3D12_RESOURCE_DESC MakeBufferDesc(size_t sizeInBytes, D3D12_RESOURCE_FLAGS flags);

					private: static void CheckGpuResourceCreation(HRESULT result);

				private: ResourceHandle::t_hash MakeUploadBuffer(const DataSource *dataSources, unsigned char numDataSources, size_t resourceSizeInBytes);

		public: ResourceHandle::t_hash MakeReadbackBuffer(size_t sizeInBytes, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

		
		public: ResourceHandle::t_hash MakeTextureWithData(const void *data, size_t width, size_t height, D3D12_RESOURCE_STATES desiredState, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

			private: ResourceHandle::t_hash MakeTextureWithDataInternal
			(
				AllocatableGpuMemory &memorySource,
				ResourceTypes allocationType,
				const void *data,
				size_t width,
				size_t height,
				DXGI_FORMAT format,
				unsigned texelSizeInBytes,
				D3D12_RESOURCE_STATES desiredState,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE
			);
			
				private: ResourceHandle::t_hash MakePlacedTextureResource
				(
					AllocatableGpuMemory &memorySource,
					ResourceTypes allocationType,
					DXGI_FORMAT format,
					size_t textureSizeInBytes,
					size_t width,
					size_t height,
					unsigned depth,
					D3D12_RESOURCE_FLAGS resourceFlags,
					D3D12_RESOURCE_STATES resourceState
				);

				private: ResourceHandle::t_hash MakeUploadTexture
				(
					const void *data,
					size_t textureSizeInBytes,
					const D3D12_SUBRESOURCE_FOOTPRINT &footprint,
					unsigned texelSizeInBytes
				);

			

		public: ResourceHandle::t_hash MakeDepthTexture(size_t width, size_t height, bool withStencil, D3D12_RESOURCE_STATES desiredState, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

		public: void Deallocate(ResourceAllocation &allocation, ResourceTypes type);

		protected: virtual void DeallocateInternal(ResourceAllocation &allocation, ResourceTypes type) {};
		

					
	};

	
}
