#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	namespace DX12
	{
		class ResourceRegistryUsingReferences
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ResourceRegistryUsingReferences)


			public: virtual void AddReference(ResourceHandle::t_hash handle) = 0;

			public: virtual void RemoveReference(ResourceHandle::t_hash handle) = 0;
			
		};

		
	}

	
}
