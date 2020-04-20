#pragma once
#include <unordered_map>
#include "Resources/ResourceRegistryReadOnly.hpp"
#include "ResourceRegistryUsingReferences.hpp"


namespace Renderer
{
	namespace DX12
	{		
		class ResourceRegistryDetachedReference : public ResourceRegistryReadOnly, public ResourceRegistryUsingReferences
		{
			private: union Bucket
			{
				ID3D12RootSignature *signature;
				ID3D12PipelineState *pipeline;
				D3D12_GPU_VIRTUAL_ADDRESS resourceGPUAddress;
			};

			private: std::unordered_map<ResourceHandle::t_hash, Bucket> resources;

			private: ResourceRegistryReadOnly *referencedRegistry;



			public:	explicit ResourceRegistryDetachedReference(ResourceRegistryReadOnly &referencedRegistry);
			

			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(ResourceHandle::t_hash handle) override;


			public: virtual void AddReference(ResourceHandle::t_hash handle) override;

			public: virtual void RemoveReference(ResourceHandle::t_hash) override {}

						
			public: void AddPso(ResourceHandle::t_hash handle, ID3D12PipelineState *pipeline);

			public: void AddSignature(ResourceHandle::t_hash handle, ID3D12RootSignature *signature);

			public: void AddResourceGPUVirtualAddress(ResourceHandle::t_hash handle, D3D12_GPU_VIRTUAL_ADDRESS address);

			public: void Empty();
			
		};

		
	}
	
	
}
