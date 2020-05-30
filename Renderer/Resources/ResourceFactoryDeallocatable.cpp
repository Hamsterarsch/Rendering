#include "Resources/ResourceFactoryDeallocatable.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Resources/ResourceTypes.hpp"


namespace Renderer
{
	namespace DX12
	{
		ResourceFactoryDeallocatable::ResourceFactoryDeallocatable
		(
			DeviceResources *resources, 
			Queue *queue,
			UniquePtr<DeallocatableGpuMemory> &&bufferMemory,
			UniquePtr<DeallocatableGpuMemory> &&textureMemory
		)	:
			ResourceFactory{ resources, queue, std::move(bufferMemory), std::move(textureMemory) }
		{
		}



		void ResourceFactoryDeallocatable::DeallocateInternal(ResourceAllocation &allocation, const ResourceTypes type)
		{
			switch(type)
			{
			case ResourceTypes::Buffer:
				DeallocateBuffer(allocation);
				break;
			case ResourceTypes::Texture:
				reinterpret_cast<DeallocatableGpuMemory *>(textureMemory.get())->Deallocate(allocation.allocation);													
				CheckAndReleaseResourceRefs(allocation.resource);
				break;
			default:
				ThrowIfDebug(Exception::Exception{ "Resource type missing handling in dx12 resource factory deallocation" });				
			}
			
		}

			void ResourceFactoryDeallocatable::DeallocateBuffer(ResourceAllocation &allocation)
			{
				reinterpret_cast<DeallocatableGpuMemory *>(bufferMemory.get())->Deallocate(allocation.allocation);									
				
				CheckAndReleaseResourceRefs(allocation.resource);
				
			}

				void ResourceFactoryDeallocatable::CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource)
				{
					const auto cutReferences{ resource.Reset() };
					
					if(cutReferences > 0)
					{
						ThrowIfDebug(Exception::Exception{ "Deallocated a resource allocation whose resource is still referenced" });
					}
			
				}

				
	}

	
}