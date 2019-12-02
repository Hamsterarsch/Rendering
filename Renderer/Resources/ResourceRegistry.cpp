#include "Resources/ResourceRegistry.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer
{
	namespace DX12
	{
		void ResourceRegistry::RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation)
		{
			resourceAllocations.insert(  { handle.hash, std::move(allocation) } );
			AddReference(handle.hash);
			
		}

			void ResourceRegistry::AddReference(const ResourceHandle::t_hash handle)
			{
				resourceReferences.insert(handle);
				
			}


		
		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			resourceReferences.erase(resourceReferences.find(handle));

			if(resourceReferences.count(handle) == 0)
			{
				resourceAllocations.erase(handle);				
			}
			
		}

		

		bool ResourceRegistry::ResourceIsNotRegistered(const ResourceHandle::t_hash handle)
		{
			return resourceAllocations.find(handle) == resourceAllocations.end();
			
		}


		
		DxPtr<ID3D12Resource> ResourceRegistry::GetResource(const ResourceHandle::t_hash handle)
		{
			const auto allocation{ resourceAllocations.find(handle) };

			if(allocation == resourceAllocations.end())
			{
				throw Exception::Exception{ "A resource with this handle does not exist in this dx12 resource registry" };
			}
			
			return allocation->second.resource;
			
		}

		
	}
	
	
}
