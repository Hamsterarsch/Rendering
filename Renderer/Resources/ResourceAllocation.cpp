#include "Resources/ResourceAllocation.hpp"
#include "Resources/ResourceFactory.hpp"

namespace Renderer::DX12
{
	ResourceAllocation::ResourceAllocation(ResourceFactory *owner, ResourceTypes type) :
		owner{ owner },
		type{ type },
		resource{ nullptr },
		allocation{}
	{
	}


	
	ResourceAllocation::~ResourceAllocation()
	{
		Free();
		
	}
	
		void ResourceAllocation::Free()
		{
			if(IsValid())
			{
				owner->Deallocate(*this, type);
			}	
		
		}
		
			bool ResourceAllocation::IsValid() const
			{
				return resource != nullptr;
		
			}

	

	ResourceAllocation::ResourceAllocation(ResourceAllocation &&other) noexcept :
		ResourceAllocation(nullptr, ResourceTypes::Buffer)
	{
		*this = std::move(other);
		
	}


	
	ResourceAllocation &ResourceAllocation::operator=(ResourceAllocation &&other) noexcept
	{
		if(this == &other)
		{
			return *this;
			
		}

		Free();
		
		owner = std::move(other.owner);
		other.owner = nullptr;
		
		type = std::move(other.type);
		resource = std::move(other.resource);			
		allocation = std::move(other.allocation);
		
		return *this;
		
	}

	
	
}
