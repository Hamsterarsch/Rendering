#pragma once
#include "ContainsReferences.hpp"
#include "Resources/UsesReferences.hpp"
#include "DescriptorAllocator.hpp"
#include <vector>


namespace Renderer::DX12
{
	class  ReferenceAwareDescriptorAllocator : public ContainsReferences
	{
		private: std::vector<ResourceHandle::t_hash> referencesResources;

		public: DescriptorAllocator allocator;


		public: void AddReferenceTo(ResourceHandle::t_hash resource);
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;

	};


	inline void ReferenceAwareDescriptorAllocator::AddReferenceTo(ResourceHandle::t_hash resource)
	{
		if(resource)
		{
			referencesResources.emplace_back(resource);			
		}		
		
	}

	inline void ReferenceAwareDescriptorAllocator::ExecuteOperationOnResourceReferences
	(UsesReferences& registry, void( UsesReferences::* operation)(size_t))
	{
		for(auto &&resource : referencesResources)
		{
			(registry.*operation)(resource);
		}
		
	}

	
}
