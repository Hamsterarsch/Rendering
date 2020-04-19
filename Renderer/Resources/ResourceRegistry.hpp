#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Resources/ResourceRegistryReadOnly.hpp"
#include "ResourceRegistryUsingReferences.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"
#include "Resources/ResourceAllocation.hpp"


namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class ResourceRegistry : public ResourceRegistryReadOnly, public ResourceRegistryUsingReferences
		{						
			private: std::unordered_map<ResourceHandle::t_hash, ResourceAllocation> resourceAllocations;
			
			private: std::unordered_map<ResourceHandle::t_hash, size_t> resourceReferences;

			private: std::unordered_set<ResourceHandle::t_hash> unreferencedResources;
			
			private: std::unordered_map<ResourceHandle::t_hash, RootSignatureData> rootSignatures;

			private: std::unordered_map<ResourceHandle::t_hash, DxPtr<ID3D12PipelineState>> pipelineStates;

			
			
			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;
			
			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;
			
			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(ResourceHandle::t_hash handle) override;


			
			public: void RegisterPso(ResourceHandle::t_hash handle, const DxPtr<ID3D12PipelineState> &pipelineState);
			
			public: virtual void AddReference(ResourceHandle::t_hash handle) override;
					
			public: void RegisterSignature(ResourceHandle::t_hash handle, RootSignatureData &&signatureData);

			public: void RegisterResource(size_t handle, ResourceAllocation &&allocation);


			
			public: virtual void RemoveReference(ResourceHandle::t_hash handle) override;
			
				private: static bool ThereAreNoReferencesIn(const decltype(resourceReferences)::const_iterator &referenceBucket);
			
			public: void PurgeUnreferencedResources();

				private: void RemoveEntity(ResourceHandle::t_hash hash);
			
			public: bool HandleIsInvalid(ResourceHandle::t_hash handle);

			public: bool HandleIsUnreferenced(ResourceHandle::t_hash handle);
			

			private: const RootSignatureData &GetSignatureDataRef(ResourceHandle::t_hash handle) const;
												
		};


	}
	
	
}
