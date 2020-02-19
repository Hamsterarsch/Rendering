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
			UniquePtr<DeallocatableGpuMemory> &&memory
		)	:
			ResourceFactory{ resources, queue, std::move(memory) }
		{
		}



		void ResourceFactoryDeallocatable::Deallocate(ResourceAllocation &allocation, const ResourceTypes type)
		{
			switch(type)
			{
			case ResourceTypes::Buffer:
				DeallocateBuffer(allocation);
				break;
			default:
				throw Exception::Exception{ "Resource type missing handling in dx12 resource factory deallocation" };				
			}
			
		}

			void ResourceFactoryDeallocatable::DeallocateBuffer(ResourceAllocation &allocation)
			{
				reinterpret_cast<DeallocatableGpuMemory *>(memory.get())->Deallocate(allocation.allocation);				
				CheckAndReleaseResourceRefs(allocation.resource);
				
			}

				void ResourceFactoryDeallocatable::CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource)
				{
					const auto cutReferences{ resource.Reset() };
					if(cutReferences > 0)
					{
						throw Exception::Exception{ "Deallocated a resource allocation whose resource is still referenced" };//todo: this could be done in debugging only
					}
			
				}

				
	}

	
}