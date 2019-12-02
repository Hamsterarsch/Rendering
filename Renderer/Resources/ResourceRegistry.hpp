#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceAllocation.hpp"


struct ID3D12PipelineState;
struct ID3D12RootSignature;

namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class ResourceRegistry
		{			
			private: std::unordered_map<ResourceHandle::t_hash, ResourceAllocation> resourceAllocations;
			
			private: std::unordered_multiset<ResourceHandle::t_hash> resourceReferences;



			public: void RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation);

			public: void AddReference(ResourceHandle::t_hash handle);

			public: void RemoveReference(ResourceHandle::t_hash handle);

			public: bool ResourceIsNotRegistered(ResourceHandle::t_hash handle);

			public: DxPtr<ID3D12Resource> GetResource(ResourceHandle::t_hash handle);

			public: DxPtr<ID3D12PipelineState> GetPso(ResourceHandle::t_hash handle);

			public: DxPtr<ID3D12RootSignature> GetSignature(ResourceHandle::t_hash handle);

			
			
									
		};
		
		
	}
	
	
}
