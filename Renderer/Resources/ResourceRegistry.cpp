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
				++InsertOrFindReferenceData(handle)->second;
				
			}

				decltype(ResourceRegistry::resourceReferences)::iterator ResourceRegistry::InsertOrFindReferenceData
				(
					const ResourceHandle::t_hash hash
				)
				{
					auto referenceData{ resourceReferences.find(hash) };
					if(referenceData == resourceReferences.end())
					{
						return resourceReferences.insert( {hash, 0} );
						
					}

					return referenceData;
				
				}


		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			auto referenceData{ resourceReferences.find(handle) };
			--referenceData->second;
			
			if(referenceData->second <= 0)
			{
				ResourceHandle realHandle{ handle };
				RemoveEntity(realHandle);
			}
			
		}

			void ResourceRegistry::RemoveEntity(const ResourceHandle &handle)
			{
				switch(handle.GetResourceType())
				{
				case ResourceTypes::Mesh: 
				case ResourceTypes::Texture: 
				case ResourceTypes::Buffer:
					resourceAllocations.erase(handle.hash);
					break;
				case ResourceTypes::Pso:
					pipelineStates.erase(handle.hash);
					break;
				case ResourceTypes::Signature:
					rootSignatures.erase(handle.hash);
					break;
				default: throw Exception::Exception{ "No removal handling for this resource type in dx12 resource registry" };
				}
				
			}


		
		bool ResourceRegistry::HandleIsInvalid(const ResourceHandle::t_hash hash)
		{	
			auto found{ resourceReferences.find(hash) };
			
			if(found == resourceReferences.end())
			{
				return true;
			}

			if(found->second == 0)
			{
				throw;
			}
									
			return false;			
			
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
			AddReference(handle);
			
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
			AddReference(handle);
			
		}

		ID3D12PipelineState *ResourceRegistry::GetPso(const ResourceHandle::t_hash handle) const
		{
			return pipelineStates.at(handle).Get();
			
		}

		
	}
	
	
}
