#include "Resources/ResourceRegistry.hpp"
#include "Descriptor/DescriptorMemory.hpp"



namespace Renderer::DX12
{
	ResourceRegistry::ResourceRegistry() 		
	{
		registryDescriptor.SetOnEntityPurged([&r = registryResource](ReferenceAwareDescriptorAllocator &entity)
		{
			entity.ExecuteOperationOnResourceReferences(r, &UsesReferences::RemoveReference);
		});
		
		registryRawDescriptor.SetOnEntityPurged([&r = registryResource](RawDescriptorReference &entity)
		{
			entity.memorySource->RetireDepthStencilDescriptor(entity.descriptor);
			r.RemoveReference(entity.referencedResource);
			
		});
		
	}

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

		if(handleType == ResourceHandle::t_resourceTypes::DescriptorAllocator)
		{
			return registryDescriptor.IsHandleUnknown(handle);
		}
		
		return registryResource.IsHandleUnknown(handle);
		
	}

	

	ResourceHandle::t_hash ResourceRegistry::Register(ResourceAllocation &&allocation)
	{
		const auto handle{ handleProvider.MakeHandle(allocation.GetType()) };
		registryResource.Register(handle, std::move(allocation));
		return handle;
		
	}


	
	ResourceHandle::t_hash ResourceRegistry::Register(RootSignatureData &&signature)
	{			
		const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::Signature) };
		registrySignature.Register(handle, std::move(signature));
		return handle;
		
	}


	
	ResourceHandle::t_hash ResourceRegistry::Register(DxPtr<ID3D12PipelineState> &&pipeline)
	{					
		const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::Pso) };
		registryPso.Register(handle, std::move(pipeline));
		return handle;
		
	}


	
	ResourceHandle::t_hash ResourceRegistry::Register(UniquePtr<RHA::DX12::WindowSurface> &&surface)
	{
		const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::WindowSurface) };
		registryWindowSurface.Register(handle, std::move(surface));

		return handle;
		
	}


	
	ResourceHandle::t_hash ResourceRegistry::Register(ReferenceAwareDescriptorAllocator &&allocator)
	{
		const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::DescriptorAllocator) };

		allocator.ExecuteOperationOnResourceReferences(registryResource, &UsesReferences::AddReference);
		registryDescriptor.Register(handle, std::move(allocator));

		return handle;
		
	}


	
	ResourceHandle::t_hash ResourceRegistry::Register(RawDescriptorReference &&descriptor)
	{
		const auto handle{ handleProvider.MakeHandle(ResourceHandle::t_resourceTypes::RawDescriptor) };
		registryRawDescriptor.Register(handle, std::move(descriptor));
		registryResource.AddReference(descriptor.referencedResource);
		
		return handle;
		
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


	
	bool ResourceRegistry::IsWindowSurfaceReferenced(const ResourceHandle::t_hash handle) const
	{
		return registryWindowSurface.GetReferenceCount(handle) > 0;
		
	}



	DescriptorAllocator &ResourceRegistry::GetDescriptorAllocator(const ResourceHandle::t_hash handle)
	{
		return registryDescriptor.Get(handle).allocator;
		
	}


	
	D3D12_CPU_DESCRIPTOR_HANDLE ResourceRegistry::GetRawDescriptor(const ResourceHandle::t_hash handle)
	{
		return registryRawDescriptor.Get(handle).descriptor;
		
	}



	RHA::DX12::WindowSurface *ResourceRegistry::GetSurface(const ResourceHandle::t_hash handle)
	{
		return registryWindowSurface.Get(handle);
		
	}



	size_t ResourceRegistry::GetSignatureCbvOffset(const ResourceHandle::t_hash signature, const unsigned tableIndex, const size_t cbvOrdinal) 
	{
		return GetSignatureOffset(signature, tableIndex, cbvOrdinal, &TableLayout::GetCbvOffset);
		
	}
		   
		size_t ResourceRegistry::GetSignatureOffset
		(
			const ResourceHandle::t_hash signature,  
			const unsigned tableIndex,
			const size_t ordinal,
			size_t( TableLayout:: *const getter)(unsigned short) const
		) 
		{
			return (registrySignature.Get(signature).layouts.at(tableIndex).*getter)(ordinal);
		
		}



	size_t ResourceRegistry::GetSignatureSrvOffset(const ResourceHandle::t_hash signature, const unsigned tableIndex, const size_t srvOrdinal) 
	{
		return GetSignatureOffset(signature, tableIndex, srvOrdinal, &TableLayout::GetSrvOffset);
		
	}



	size_t ResourceRegistry::GetSignatureUavOffset(const ResourceHandle::t_hash signature, const unsigned tableIndex, const size_t uavOrdinal) 
	{
		return GetSignatureOffset(signature, tableIndex, uavOrdinal, &TableLayout::GetUavOffset);
		
	}


	
	void ResourceRegistry::RetireHandle(const ResourceHandle::t_hash handle)
	{
		RemoveReference(handle);
		handlesToRetire.push_front(handle);
		
	}


	
	void ResourceRegistry::PurgeUnreferencedEntities()
	{
		registryDescriptor.PurgeUnreferencedEntities();
		registryResource.PurgeUnreferencedEntities();		
		registryPso.PurgeUnreferencedEntities();
		registrySignature.PurgeUnreferencedEntities();			
		registryRawDescriptor.PurgeUnreferencedEntities();
		
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
			void (UsesReferences:: *const operation)(ResourceHandle::t_hash)
		)
		{
			if(handle == 0)
			{
				return;
			}

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

			if(handleType == ResourceHandle::t_resourceTypes::WindowSurface)
			{
				(registryWindowSurface.*operation)(handle);
				return;
				
			}

			if(handleType == ResourceHandle::t_resourceTypes::DescriptorAllocator)
			{
				(registryDescriptor.*operation)(handle);
				return;
				
			}

			if(handleType == ResourceHandle::t_resourceTypes::RawDescriptor)
			{
				(registryRawDescriptor.*operation)(handle);
				return;
			}

			(registryResource.*operation)(handle);
		
		}


	void ResourceRegistry::RemoveReference(const ResourceHandle::t_hash handle)
	{
		ExecuteReferenceOperationOnCorrectRegistry(handle, &UsesReferences::RemoveReference);
		
	}

	
}
