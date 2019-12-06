#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Resources/ResourceHandle.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"
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

			private: std::unordered_map<ResourceHandle::t_hash, RootSignatureData> rootSignatures;

			private: std::unordered_map<ResourceHandle::t_hash, DxPtr<ID3D12PipelineState>> pipelineStates;



			public: void RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation);

			public: void AddReference(ResourceHandle::t_hash handle);

			public: void RemoveReference(ResourceHandle::t_hash handle);

				private: void RemoveEntity(const ResourceHandle &handle);

			
			public: bool ResourceIsNotRegistered(ResourceHandle::t_hash handle);

			public: DxPtr<ID3D12Resource> GetResource(ResourceHandle::t_hash handle);


			public: void RegisterSignature(ResourceHandle::t_hash handle, RootSignatureData &&signatureData);
			
			public: const RootSignatureData &GetSignatureDataRef(ResourceHandle::t_hash handle) const;

			public: ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle);


			public: void RegisterPso(ResourceHandle::t_hash handle, const DxPtr<ID3D12PipelineState> &pipelineState);
			
			public: DxPtr<ID3D12PipelineState> GetPso(ResourceHandle::t_hash handle) const;
			

			
			
									
		};


	}
	
	
}
