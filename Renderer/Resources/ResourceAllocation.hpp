#pragma once
#include "DX12/HeapAllocation.hpp"
#include "DxPtrTypes.hpp"
#include "Resources/ResourceHandle.hpp"

struct ID3D12Resource;

namespace Renderer::DX12
{		
	class ResourceAllocation
	{
		private: class ResourceFactory *owner;

		private: ResourceHandle::t_resourceTypes type;
		

		public: DxPtr<ID3D12Resource> resource;
		
		public: RHA::DX12::HeapAllocation allocation;


								   			
		public: ResourceAllocation(ResourceFactory *owner, ResourceTypes type);
		
		public: ~ResourceAllocation();

		public: void Free();

			private: void Invalidate();

		public: bool IsValid() const;
		
		public: ResourceAllocation(ResourceAllocation &&other) noexcept;

		public: ResourceAllocation &operator=(ResourceAllocation &&rhs) noexcept;

		public: ResourceHandle::t_resourceTypes GetType() const { return type; }
		
							
		ResourceAllocation(const ResourceAllocation &other) = delete;
					
		ResourceAllocation &operator=(const ResourceAllocation &other) = delete;

	};
	
	
}
