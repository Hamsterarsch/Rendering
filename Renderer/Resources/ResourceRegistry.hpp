#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"
#include "Resources/ResourceAllocation.hpp"
#include <mutex>


struct ID3D12PipelineState;
struct ID3D12RootSignature;


namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class ResourceRegistryReadOnly
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ResourceRegistryReadOnly)

			
			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) = 0;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) = 0; 

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(ResourceHandle::t_hash handle) = 0;
			
		};


		
		class ResourceRegistry : public ResourceRegistryReadOnly
		{						
			private: std::unordered_map<ResourceHandle::t_hash, ResourceAllocation> resourceAllocations;
			
			private: std::unordered_map<ResourceHandle::t_hash, size_t> resourceReferences;

			private: std::unordered_set<ResourceHandle::t_hash> unreferencedResources;
			
			private: std::unordered_map<ResourceHandle::t_hash, RootSignatureData> rootSignatures;

			private: std::unordered_map<ResourceHandle::t_hash, DxPtr<ID3D12PipelineState>> pipelineStates;

			
			private: mutable std::mutex referenceMutex, allocationMutex, signatureMutex, pipelineMutex;


			
			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;
			
			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;
			
			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(ResourceHandle::t_hash handle) override;


			
			public: void RegisterPso(ResourceHandle::t_hash handle, const DxPtr<ID3D12PipelineState> &pipelineState);
			
			public: void AddReference(ResourceHandle::t_hash handle);
					
			public: void RegisterSignature(ResourceHandle::t_hash handle, RootSignatureData &&signatureData);

			public: void RegisterResource(size_t handle, ResourceAllocation &&allocation);


			
			public: void RemoveReference(ResourceHandle::t_hash handle);
			
				private: static bool ThereAreNoReferencesIn(const decltype(resourceReferences)::const_iterator &referenceBucket);
			
			public: void PurgeUnreferencedResources();

				private: void RemoveEntity(ResourceHandle::t_hash hash);
			
			public: bool HandleIsInvalid(ResourceHandle::t_hash handle);

			public: bool HandleIsUnreferenced(ResourceHandle::t_hash handle);
			

			private: const RootSignatureData &GetSignatureDataRef(ResourceHandle::t_hash handle) const;
												
		};


	}
	
	
}
