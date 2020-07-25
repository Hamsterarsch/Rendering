#pragma once
#include "Resources/ResourceFactory.hpp"


namespace Renderer::DX12
{
	class ResourceFactoryDeallocatable : public ResourceFactory
	{
		public: ResourceFactoryDeallocatable
		(
			DeviceResources *resources,
			Queue *queue,
			UniquePtr<DeallocatableGpuMemory> &&bufferMemory,
			UniquePtr<DeallocatableGpuMemory> &&textureMemory,
			UniquePtr<DeallocatableGpuMemory> &&depthTextureMemory,
			UniquePtr<DeallocatableGpuMemory> &&bufferReadbackMemory
		);

				
		public: virtual void DeallocateInternal(ResourceAllocation &allocation, ResourceTypes type) override;
		
			private: void DeallocateFromDeallocMemory(AllocatableGpuMemory &fromMemory, ResourceAllocation &allocation);

				private: void CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource);
		
	};

	
}
