#pragma once
#include <unordered_map>
#include "Resources/HasQueriableResources.hpp"
#include "UsesReferences.hpp"


namespace Renderer
{
	namespace DX12
	{		
		class ResourceRegistryDetachedReference : public HasQueriableResources, public UsesReferences
		{
			private: union Bucket
			{
				ID3D12RootSignature *signature;
				ID3D12PipelineState *pipeline;
				ID3D12Resource *resource;
			};

			private: std::unordered_map<ResourceHandle::t_hash, Bucket> resources;

			private: HasQueriableResources *referencedRegistry;



			public:	explicit ResourceRegistryDetachedReference(HasQueriableResources &referencedRegistry);

			
			public: virtual ID3D12Resource *GetResource(ResourceHandle::t_hash handle) override;

			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGpuAddress(ResourceHandle::t_hash handle) override;


			public: virtual void AddReference(ResourceHandle::t_hash handle) override;

			public: virtual void RemoveReference(ResourceHandle::t_hash) override {}

						
			public: void AddPso(ResourceHandle::t_hash handle, ID3D12PipelineState *pipeline);

			public: void AddSignature(ResourceHandle::t_hash handle, ID3D12RootSignature *signature);

			public: void AddResource(ResourceHandle::t_hash handle, ID3D12Resource *resource);

			public: void Empty();
			
		};

		
	}
	
	
}
