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


		
		ResourceAllocation::ResourceAllocation(ResourceAllocation &&other) noexcept
		{
			*this = std::move(other);
			
		}

		ResourceAllocation& ResourceAllocation::operator=(ResourceAllocation &&other) noexcept
		{
			owner = other.owner;

			resource = std::move(other.resource);
			other.resource = nullptr;

			allocation = other.allocation;
			
			return *this;
			
		}

		
	}
	
	
}
