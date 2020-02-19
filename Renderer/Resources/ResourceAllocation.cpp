#include "Resources/ResourceAllocation.hpp"
#include "Resources/ResourceFactory.hpp"

namespace Renderer
{
	namespace DX12
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
			if(resource != nullptr)
			{
				owner->Deallocate(*this, type);
			}
			
		}


		
		ResourceAllocation::ResourceAllocation(ResourceAllocation &&other) noexcept :
			owner{ std::move(other.owner) },
			type{ std::move(other.type) },
			resource{ std::move(other.resource) },
			allocation{ std::move(other.allocation) }
		{
			other.owner = nullptr;
			
		}

		ResourceAllocation& ResourceAllocation::operator=(ResourceAllocation &&other) noexcept
		{
			owner = std::move(other.owner);
			other.owner = nullptr;
			
			type = std::move(other.type);

			resource = std::move(other.resource);
			
			allocation = std::move(other.allocation);
			
			return *this;
			
		}

		
	}
	
	
}
