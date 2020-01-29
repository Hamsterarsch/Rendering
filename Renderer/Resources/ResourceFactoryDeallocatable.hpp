#pragma once
#include "Resources/ResourceFactory.hpp"


namespace Renderer
{
	namespace DX12
	{
		class ResourceFactoryDeallocatable : public ResourceFactory
		{
			public: ResourceFactoryDeallocatable(DeviceResources *resources, Queue *queue, UniquePtr<DeallocatableGpuMemory> &&memory);

					
			public: virtual void Deallocate(ResourceAllocation &allocation, ResourceTypes type) override;
			
				private: void DeallocateBuffer(ResourceAllocation &allocation);

					private: void CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource);
			
		};

		
	}
	
	
}
