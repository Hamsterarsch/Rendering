#pragma once
#include <unordered_map>
#include <unordered_set>
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
		class ResourceRegistry
		{						
			private: std::unordered_map<ResourceHandle::t_hash, ResourceAllocation> resourceAllocations;
			
			private: std::unordered_map<ResourceHandle::t_hash, size_t> resourceReferences;

			private: std::unordered_set<ResourceHandle::t_hash> unreferencedResources;
			
			private: std::unordered_map<ResourceHandle::t_hash, RootSignatureData> rootSignatures;

			private: std::unordered_map<ResourceHandle::t_hash, DxPtr<ID3D12PipelineState>> pipelineStates;

			
			private: mutable std::mutex referenceMutex, allocationMutex, signatureMutex, pipelineMutex;


			
			public: void RegisterResource(size_t handle, ResourceAllocation &&allocation);

			public: void AddReference(ResourceHandle::t_hash handle);

				private: static bool ThereAreNoReferencesIn(const decltype(resourceReferences)::const_iterator &referenceBucket);
			
				private: decltype(resourceReferences)::iterator InsertOrFindReferenceData(ResourceHandle::t_hash hash);

			public: void RemoveReference(ResourceHandle::t_hash handle);
			
			public: void PurgeUnreferencedResources();

				private: void RemoveEntity(ResourceHandle::t_hash hash);
			
			public: bool HandleIsInvalid(ResourceHandle::t_hash handle);

			public: bool HandleIsUnreferenced(ResourceHandle::t_hash handle);
			
			public: DxPtr<ID3D12Resource> GetResource(ResourceHandle::t_hash handle);


			public: void RegisterSignature(ResourceHandle::t_hash handle, RootSignatureData &&signatureData);
			
			private: const RootSignatureData &GetSignatureDataRef(ResourceHandle::t_hash handle) const;

			public: ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) const;


			public: void RegisterPso(ResourceHandle::t_hash handle, const DxPtr<ID3D12PipelineState> &pipelineState);
			
			public: ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) const;
			

			
			
									
		};


	}
	
	
}
