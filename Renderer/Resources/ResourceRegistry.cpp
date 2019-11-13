#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceRegistry.hpp"


namespace Renderer
{
	namespace DX12
	{
		void ResourceRegistry::RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation)
		{
			resourceAllocations.insert(  { handle.hash, std::move(allocation) } );
			AddReference(handle.hash);
			
		}

			void ResourceRegistry::AddReference(const size_t handle)
			{
				resourceReferences.insert(handle);
				
			}


					
		bool ResourceRegistry::ResourceIsNotRegistered(const size_t handle)
		{
			return resourceAllocations.find(handle) == resourceAllocations.end();
			
		}

		
	}
	
	
}