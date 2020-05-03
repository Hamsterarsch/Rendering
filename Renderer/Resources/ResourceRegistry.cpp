#include "Resources/ResourceRegistry.hpp"



namespace Renderer
{
	namespace DX12
	{		
		bool ResourceRegistry::IsHandleUnknown(const ResourceHandle::t_hash handle) const
		{
			const auto handleType{  ResourceHandle::GetResourceType(handle) };
			if(handleType == ResourceHandle::t_resourceTypes::Pso)
			{
				return registryPso.IsHandleUnknown(handle);
			}

			if(handleType == ResourceHandle::t_resourceTypes::Signature)
			{
				return registrySignature.IsHandleUnknown(handle);
			}

			return registryResource.IsHandleUnknown(handle);
			
		}

		

		ResourceHandle::t_hash ResourceRegistry::Register(ResourceAllocation &&allocation)
		{
			const auto handle{ handleProvider.MakeHandle(allocation.GetType()) };
			Register(handle, std::move(allocation));
			return handle;
			
		}
			   		
			void ResourceRegistry::Register(const ResourceHandle::t_hash handle, ResourceAllocation &&allocation)
			{
				registryResource.Register(handle, std::move(allocation));
				
			}


		
		ResourceHandle::t_hash ResourceRegistry::Register(RootSignatureData &&signature)
		{			
			const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::Signature) };
			Register(handle, std::move(signature));
			return handle;
			
		}


		
		void ResourceRegistry::Register(const ResourceHandle::t_hash handle, RootSignatureData &&signature)
		{
			registrySignature.Register(handle, std::move(signature));
			
		}


		
		ResourceHandle::t_hash ResourceRegistry::Register(DxPtr<ID3D12PipelineState> &&pipeline)
		{					
			const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::Pso) };
			Register(handle, std::move(pipeline));
			return handle;
			
		}


		
		void ResourceRegistry::Register(const ResourceHandle::t_hash handle, DxPtr<ID3D12PipelineState> &&pipeline)
		{
			registryPso.Register(handle, std::move(pipeline));
			
		}

		

		ID3D12Resource *ResourceRegistry::GetResource(const ResourceHandle::t_hash handle)
		{
			return registryResource.Get(handle);
			
		}


		
		D3D12_GPU_VIRTUAL_ADDRESS ResourceRegistry::GetResourceGpuAddress(const ResourceHandle::t_hash handle)
		{
			return registryResource.Get(handle)->GetGPUVirtualAddress();
			
		}

		

		ID3D12PipelineState *ResourceRegistry::GetPso(const ResourceHandle::t_hash handle)
		{
			return registryPso.Get(handle);
			
		}



		ID3D12RootSignature *ResourceRegistry::GetSignature(const ResourceHandle::t_hash handle)
		{
			return registrySignature.Get(handle).signature.Get();
			
		}



		size_t ResourceRegistry::GetSignatureCbvOffset(const ResourceHandle::t_hash handle, const size_t cbvOrdinal) 
		{
			return GetSignatureOffset(handle, cbvOrdinal, &TableLayout::GetCbvOffset);
			
		}
			   
			size_t ResourceRegistry::GetSignatureOffset
			(
				const ResourceHandle::t_hash handle, 
				const size_t ordinal,
				size_t( TableLayout:: *const getter)(unsigned short) const
			) 
			{
				return (registrySignature.Get(handle).layout.*getter)(ordinal);
			
			}



		size_t ResourceRegistry::GetSignatureSrvOffset(const ResourceHandle::t_hash handle, const size_t srvOrdinal) 
		{
			return GetSignatureOffset(handle, srvOrdinal, &TableLayout::GetSrvOffset);
			
		}



		size_t ResourceRegistry::GetSignatureUavOffset(const ResourceHandle::t_hash handle, const size_t uavOrdinal) 
		{
			return GetSignatureOffset(handle, uavOrdinal, &TableLayout::GetUavOffset);
			
		}


		
		void ResourceRegistry::RetireHandle(const ResourceHandle::t_hash handle)
		{
			handlesToRetire.push_front(handle);
			
		}


		
		void ResourceRegistry::PurgeUnreferencedEntities()
		{
			registryResource.PurgeUnreferencedEntities();
			registryPso.PurgeUnreferencedEntities();
			registrySignature.PurgeUnreferencedEntities();
						
			handlesToRetire.remove_if([ &rorch = *this](const size_t &handle)
			{
				if(rorch.IsHandleUnknown(handle))
				{
					rorch.handleProvider.RetireHandle(ResourceHandle{ handle });
					return true;					
				}
				return false;
				
			});			
			
		}


		
		void ResourceRegistry::AddReference(const ResourceHandle::t_hash handle)
		{
			ExecuteReferenceOperationOnCorrectRegistry(handle, &UsesReferences::AddReference);
			
		}

			void ResourceRegistry::ExecuteReferenceOperationOnCorrectRegistry
			(
				const ResourceHandle::t_hash handle,
				void (UsesReferences::* const operation)(ResourceHandle::t_hash)
			)
			{
				const auto handleType{  ResourceHandle::GetResourceType(handle) };
				if(handleType == ResourceHandle::t_resourceTypes::Pso)
				{
					(registryPso.*operation)(handle);
					return;
					
				}

				if(handleType == ResourceHandle::t_resourceTypes::Signature)
				{
					(registrySignature.*operation)(handle);
					return;
					
				}

				(registryResource.*operation)(handle);
			
			}


		void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
		{
			ExecuteReferenceOperationOnCorrectRegistry(handle, &UsesReferences::RemoveReference);
			
		}

		
	}
	
	
}