#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Resources/ResourceAllocation.hpp"


namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class ResourceRegistry
		{			
			private: std::unordered_map<size_t, ResourceAllocation> resourceAllocations;
			
			private: std::unordered_multiset<size_t> resourceReferences;



			public: void RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation);

			public: void AddReference(size_t handle);

			public: void RemoveReference(size_t handle);

			public: bool ResourceIsNotRegistered(size_t handle);
			
									
		};
		
		
	}
	
	
}
