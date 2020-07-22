#pragma once
#include "Resources/ResourceFactory.hpp"


namespace Renderer
{
	namespace DX12
	{
		class ResourceFactoryDeallocatable : public ResourceFactory
		{
			public: ResourceFactoryDeallocatable
			(
				DeviceResources *resources,
				Queue *queue,
				UniquePtr<DeallocatableGpuMemory> &&bufferMemory,
				UniquePtr<DeallocatableGpuMemory> &&textureMemory,
				UniquePtr<DeallocatableGpuMemory> &&depthTextureMemory
			);

					
			public: virtual void DeallocateInternal(ResourceAllocation &allocation, ResourceTypes type) override;
			
				private: void DeallocateBuffer(ResourceAllocation &allocation);

					private: void CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource);
			
		};

		
	}
	
	
}
