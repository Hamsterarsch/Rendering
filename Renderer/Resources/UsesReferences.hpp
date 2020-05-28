#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	class UsesReferences
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UsesReferences)


		public: virtual void AddReference(ResourceHandle::t_hash handle) = 0;

		public: virtual void RemoveReference(ResourceHandle::t_hash handle) = 0;
		
	};

	inline void Invoke(UsesReferences &instance, void(UsesReferences:: *operation)(ResourceHandle::t_hash), const ResourceHandle::t_hash hash)
	{
		(instance.*operation)(hash);
		
	}

	
}
