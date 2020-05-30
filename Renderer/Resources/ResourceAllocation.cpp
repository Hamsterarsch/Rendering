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
		if(IsValid())
		{
			Free();
		}		
		
	}
	
		void ResourceAllocation::Free()
		{
			owner->Deallocate(*this, type);
			Invalidate();			
		
		}

			void ResourceAllocation::Invalidate()
			{
				resource = nullptr;
		
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


	
	ResourceAllocation &ResourceAllocation::operator=(ResourceAllocation &&rhs) noexcept
	{
		if(this == &rhs)
		{
			return *this;
			
		}

		if(IsValid())
		{
			Free();			
		}
		
		owner = std::move(rhs.owner);
		rhs.owner = nullptr;
		
		type = std::move(rhs.type);
		resource = std::move(rhs.resource);			
		allocation = std::move(rhs.allocation);

		rhs.Invalidate();
		
		return *this;
		
	}

	
	
}
