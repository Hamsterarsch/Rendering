#include "Resources/ResourceRegistry.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer
{
	namespace DX12
	{
		void ResourceRegistry::RegisterResource(size_t handle, ResourceAllocation &&allocation)
		{
			{
			std::lock_guard<std::mutex> lock{ allocationMutex };
			resourceAllocations.insert(  { handle, std::move(allocation) } );
			}
			
			std::lock_guard<std::mutex> lock{ referenceMutex };
			unreferencedResources.emplace(handle);
			
		}


		
		void ResourceRegistry::AddReference(const ResourceHandle::t_hash handle)
		{
			std::lock_guard<std::mutex> lock{ referenceMutex };
		
			auto bucket{ InsertOrFindReferenceData(handle) };
			if(ThereAreNoReferencesIn(bucket))
			{
				unreferencedResources.erase(handle);
			}
			++bucket->second;
			
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
		
			bool ResourceRegistry::ThereAreNoReferencesIn(const decltype(resourceReferences)::const_iterator &referenceBucket)
			{
				return referenceBucket->second <= 0;
			
			}

		

		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			std::lock_guard<std::mutex> lock{ referenceMutex };
			auto referenceData{ resourceReferences.find(handle) };
			--referenceData->second;
			
			if(ThereAreNoReferencesIn(referenceData))
			{
				unreferencedResources.emplace(handle);
				resourceReferences.erase(referenceData);
			}
			
		}
				


		void ResourceRegistry::PurgeUnreferencedResources()
		{
			std::lock_guard<std::mutex> lockReferences{ referenceMutex };

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
					std::lock_guard<std::mutex> lock{ allocationMutex };
					resourceAllocations.erase(handle.hash);
					}
					break;
				case ResourceTypes::Pso:
					{
					std::lock_guard<std::mutex> lock{ pipelineMutex };
					pipelineStates.erase(handle.hash);
					}
					break;
				case ResourceTypes::Signature:
					{
					std::lock_guard<std::mutex> lock{ signatureMutex };
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
			
			std::lock_guard<std::mutex> lock{ referenceMutex };
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


		
		bool ResourceRegistry::HandleIsUnreferenced(const ResourceHandle::t_hash handle)
		{
			std::lock_guard<std::mutex> lock{ referenceMutex };
			
			return resourceReferences.find(handle) == resourceReferences.end() && unreferencedResources.find(handle) == unreferencedResources.end();
			
		}

		

		DxPtr<ID3D12Resource> ResourceRegistry::GetResource(const ResourceHandle::t_hash handle)
		{
			std::lock_guard<std::mutex> lock{ allocationMutex };
			const auto allocation{ resourceAllocations.find(handle) };

			if(allocation == resourceAllocations.end())
			{
				throw Exception::Exception{ "A resource with this handle does not exist in this dx12 resource registry" };
			}
			
			return allocation->second.resource;
			
		}


		
		void ResourceRegistry::RegisterSignature(const ResourceHandle::t_hash handle, RootSignatureData &&signatureData)
		{
			std::lock_guard<std::mutex> lock{ signatureMutex };
			rootSignatures.insert( {handle, std::move(signatureData)} );
			AddReference(handle);
			
		}


		
		const RootSignatureData &ResourceRegistry::GetSignatureDataRef(const ResourceHandle::t_hash handle) const
		{//consider synchronization when in use
			return rootSignatures.at(handle);
			
		}


		
		ID3D12RootSignature *ResourceRegistry::GetSignature(ResourceHandle::t_hash handle) const
		{
			std::lock_guard<std::mutex> lock{ signatureMutex };
			return rootSignatures.at(handle).signature.Get();
						
		}


		
		void ResourceRegistry::RegisterPso
		(
			const ResourceHandle::t_hash handle,
			const DxPtr<ID3D12PipelineState> &pipelineState
		)
		{
			std::lock_guard<std::mutex> lock{ pipelineMutex };
			pipelineStates.insert( {handle, pipelineState} );
			AddReference(handle);
			
		}


		
		ID3D12PipelineState *ResourceRegistry::GetPso(const ResourceHandle::t_hash handle) const
		{
			std::lock_guard<std::mutex> lock{ pipelineMutex };
			return pipelineStates.at(handle).Get();
			
		}

		
	}
	
	
}
