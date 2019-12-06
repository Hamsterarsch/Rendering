#include "Resources/ResourceRegistry.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer
{
	namespace DX12
	{
		void ResourceRegistry::RegisterResource(const ResourceHandle &handle, ResourceAllocation &&allocation)
		{
			resourceAllocations.insert(  { handle.hash, std::move(allocation) } );
			AddReference(handle.hash);
			
		}

			void ResourceRegistry::AddReference(const ResourceHandle::t_hash handle)
			{
				resourceReferences.insert(handle);
				
			}


		
		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			resourceReferences.erase(resourceReferences.find(handle));

			if(resourceReferences.count(handle) == 0)
			{
				resourceAllocations.erase(handle);				
			}
			
		}

		

		bool ResourceRegistry::ResourceIsNotRegistered(const ResourceHandle::t_hash handle)
		{
			return resourceAllocations.find(handle) == resourceAllocations.end();
			
		}


		
		DxPtr<ID3D12Resource> ResourceRegistry::GetResource(const ResourceHandle::t_hash handle)
		{
			const auto allocation{ resourceAllocations.find(handle) };

			if(allocation == resourceAllocations.end())
			{
				throw Exception::Exception{ "A resource with this handle does not exist in this dx12 resource registry" };
			}
			
			return allocation->second.resource;
			
		}


		
		void ResourceRegistry::RegisterSignature(const ResourceHandle::t_hash handle, RootSignatureData &&signatureData)
		{
			rootSignatures.insert( {handle, std::move(signatureData)} );
									
		}

		const RootSignatureData &ResourceRegistry::GetSignatureDataRef(const ResourceHandle::t_hash handle) const
		{
			return rootSignatures.at(handle);
			
		}

		ID3D12RootSignature *ResourceRegistry::GetSignature(ResourceHandle::t_hash handle)
		{
			return rootSignatures.at(handle).signature.Get();
			
		}


		
		void ResourceRegistry::RegisterPso
		(
			const ResourceHandle::t_hash handle,
			const DxPtr<ID3D12PipelineState> &pipelineState
		)
		{
			pipelineStates.insert( {handle, pipelineState} );
			
		}

		DxPtr<ID3D12PipelineState> ResourceRegistry::GetPso(const ResourceHandle::t_hash handle) const
		{
			return pipelineStates.at(handle);
			
		}

		
	}
	
	
}
