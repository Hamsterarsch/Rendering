#include "Resources/ResourceRegistryDetachedReference.hpp"
#include "Shared/Exception/OutOfRangeException.hpp"


namespace Renderer
{
	namespace DX12
	{
		ResourceRegistryDetachedReference::ResourceRegistryDetachedReference
		(
			HasQueriableResources &referencedRegistry
		) :
		referencedRegistry{ &referencedRegistry }
		{
		}


		
		ID3D12Resource *ResourceRegistryDetachedReference::GetResource(const ResourceHandle::t_hash handle)
		{
			return resources.at(handle).resource;
		}


		ID3D12PipelineState *ResourceRegistryDetachedReference::GetPso(const ResourceHandle::t_hash handle)
		{
			return resources.at(handle).pipeline;
			
		}


		
		ID3D12RootSignature *ResourceRegistryDetachedReference::GetSignature(const ResourceHandle::t_hash handle)
		{
			return resources.at(handle).signature;
			
		}


		
		D3D12_GPU_VIRTUAL_ADDRESS ResourceRegistryDetachedReference::GetResourceGpuAddress(const ResourceHandle::t_hash handle)
		{
			return resources.at(handle).resource->GetGPUVirtualAddress();
			
		}


		
		void ResourceRegistryDetachedReference::AddReference(const ResourceHandle::t_hash handle)
		{
			switch(ResourceHandle::GetResourceType(handle))
			{
			case ResourceTypes::Mesh: 
			case ResourceTypes::Buffer: 
			case ResourceTypes::Texture:
				this->AddResource(handle, referencedRegistry->GetResource(handle));
				break;
			case ResourceTypes::Pso:
				this->AddPso(handle, referencedRegistry->GetPso(handle));
				break;
			case ResourceTypes::Signature:
				this->AddSignature(handle, referencedRegistry->GetSignature(handle));
				break;
			default: throw Exception::OutOfRange{"Resource Registry Detached Reference Missing Switch Case"};
			}
					   			
		}

		

		void ResourceRegistryDetachedReference::AddPso(const ResourceHandle::t_hash handle, ID3D12PipelineState *pipeline)
		{
			Bucket bucket;
			bucket.pipeline = pipeline;
			
			resources[handle] = bucket;
			
		}


		
		void ResourceRegistryDetachedReference::AddSignature(const ResourceHandle::t_hash handle, ID3D12RootSignature *signature)
		{			
			Bucket bucket;
			bucket.signature = signature;
			
			resources[handle] = bucket;
			
		}


				
		void ResourceRegistryDetachedReference::AddResource(const ResourceHandle::t_hash handle, ID3D12Resource *resource)
		{			
			Bucket bucket;
			bucket.resource = resource;
			
			resources[handle] = bucket;
			
		}

		void ResourceRegistryDetachedReference::Empty()
		{
			resources.erase(resources.begin(), resources.end());
			
		}

		
	}
	
	
}
