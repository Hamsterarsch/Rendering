#include "Resources/ResourceRegistry.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer
{
	namespace DX12
	{
		ID3D12PipelineState *ResourceRegistry::GetPso(const ResourceHandle::t_hash handle) 
		{
			return pipelineStates.at(handle).Get();
			
		}


		
		ID3D12RootSignature *ResourceRegistry::GetSignature(ResourceHandle::t_hash handle) 
		{
			return rootSignatures.at(handle).signature.Get();
						
		}


		
		D3D12_GPU_VIRTUAL_ADDRESS ResourceRegistry::GetResourceGPUVirtualAddress(const ResourceHandle::t_hash handle) 
		{
			const auto allocation{ resourceAllocations.find(handle) };

			if(allocation == resourceAllocations.end())
			{
				throw Exception::Exception{ "A resource with this handle does not exist in this dx12 resource registry" };
			}
			
			return allocation->second.resource->GetGPUVirtualAddress();
			
		}


		
		ID3D12Resource *ResourceRegistry::GetResource(const ResourceHandle::t_hash handle)
		{
			return resourceAllocations.at(handle).resource.Get();
			
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

			void ResourceRegistry::AddReference(const ResourceHandle::t_hash handle)
			{
				auto &referenceCount{ resourceReferences[handle] };
				if(referenceCount == 0)
				{
					unreferencedResources.erase(handle);
				}
				++referenceCount;
				
			}


		
		void ResourceRegistry::RegisterSignature(const ResourceHandle::t_hash handle, RootSignatureData &&signatureData)
		{
			rootSignatures.insert( {handle, std::move(signatureData)} );
			AddReference(handle);
			
		}


		
		void ResourceRegistry::RegisterResource(size_t handle, ResourceAllocation &&allocation)
		{
			resourceAllocations.insert(  { handle, std::move(allocation) } );
			unreferencedResources.emplace(handle);
			
		}
		

		
		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			auto referenceData{ resourceReferences.find(handle) };
			--referenceData->second;
			
			if(ThereAreNoReferencesIn(referenceData))
			{
				unreferencedResources.emplace(handle);
				resourceReferences.erase(referenceData);
			}
			
		}

			bool ResourceRegistry::ThereAreNoReferencesIn(const decltype(resourceReferences)::const_iterator &referenceBucket)
			{
				return referenceBucket->second <= 0;
			
			}
				


		void ResourceRegistry::PurgeUnreferencedResources()
		{
			for(auto &&unreferencedHandle : unreferencedResources)
			{				
				RemoveEntity(unreferencedHandle);				
			}

			unreferencedResources.clear();
			
		}
		
			void ResourceRegistry::RemoveEntity(const ResourceHandle::t_hash hash)
			{
				const ResourceHandle handle{ hash };
			
				switch(handle.GetResourceType())
				{
				case ResourceTypes::Mesh: 
				case ResourceTypes::Texture: 
				case ResourceTypes::Buffer:
					{
					resourceAllocations.erase(handle.hash);
					}
					break;
				case ResourceTypes::Pso:
					{
					pipelineStates.erase(handle.hash);
					}
					break;
				case ResourceTypes::Signature:
					{
					rootSignatures.erase(handle.hash);
					}
					break;
				default: throw Exception::Exception{ "No removal handling for this resource type in dx12 resource registry" };
				}
				
			}


		
		bool ResourceRegistry::HandleIsInvalid(const ResourceHandle::t_hash handle)
		{
			if(handle == 0)
			{
				return true;
			}
			
			auto foundReferenceData{ resourceReferences.find(handle) };
						
			if(foundReferenceData == resourceReferences.end())
			{
				auto foundUnreferenced{ unreferencedResources.count(handle) };
				if(foundUnreferenced)
				{
					return false;
				}				
				return true;
			}

			if(ThereAreNoReferencesIn(foundReferenceData))
			{
				throw;
			}
									
			return false;			
			
		}


		
		bool ResourceRegistry::IsHandleUnknown(const ResourceHandle::t_hash handle)
		{
			return !(resourceReferences.find(handle) != resourceReferences.end() || unreferencedResources.find(handle) != unreferencedResources.end());
			
		}



		
		size_t ResourceRegistry::GetSignatureCbvOffset(const ResourceHandle::t_hash handle, const size_t cbvOrdinal) const
		{
			return GetSignatureOffset(handle, cbvOrdinal, &TableLayout::GetCbvOffset);
			
		}

			size_t ResourceRegistry::GetSignatureOffset
			(
				const ResourceHandle::t_hash handle,
				const size_t ordinal,
				size_t (TableLayout:: *const getter)(unsigned short) const
			) const
			{
				return (rootSignatures.at(handle).layout.*getter)(ordinal);
			
			}


		
		size_t ResourceRegistry::GetSignatureSrvOffset(const ResourceHandle::t_hash handle, size_t srvOrdinal) const
		{
			return GetSignatureOffset(handle, srvOrdinal, &TableLayout::GetSrvOffset);
			
		}


		
		size_t ResourceRegistry::GetSignatureUavOffset(const ResourceHandle::t_hash handle, const size_t uavOrdinal) const
		{
			return GetSignatureOffset(handle, uavOrdinal, &TableLayout::GetUavOffset);
			
		}

		
	}
	
	
}
