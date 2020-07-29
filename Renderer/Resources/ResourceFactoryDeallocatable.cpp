#include "Resources/ResourceFactoryDeallocatable.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Resources/ResourceTypes.hpp"


namespace Renderer::DX12
{
	ResourceFactoryDeallocatable::ResourceFactoryDeallocatable
	(
		DeviceResources *resources, 
		RendererFacade &renderer,
		ResourceRegistry &registry,
		UniquePtr<DeallocatableGpuMemory> &&bufferMemory,
		UniquePtr<DeallocatableGpuMemory> &&textureMemory,
		UniquePtr<DeallocatableGpuMemory> &&depthTextureMemory,
		UniquePtr<DeallocatableGpuMemory> &&bufferReadbackMemory,
		UniquePtr<DeallocatableGpuMemory> &&uploadMemory
	)	:
		ResourceFactory
		{
			resources,
			renderer,
			registry,
			std::move(bufferMemory),
			std::move(textureMemory),
			std::move(depthTextureMemory),
			std::move(bufferReadbackMemory),
			std::move(uploadMemory)
		}
	{
	}



	void ResourceFactoryDeallocatable::DeallocateInternal(ResourceAllocation &allocation, const ResourceTypes type)
	{
		switch(type)
		{
		case ResourceTypes::Buffer:
			DeallocateFromDeallocMemory(*bufferMemory, allocation);		
			break;
		case ResourceTypes::Texture:
			DeallocateFromDeallocMemory(*textureMemory, allocation);													
			break;
		case ResourceTypes::DepthTexture:
			DeallocateFromDeallocMemory(*depthTextureMemory, allocation);
			break;
		case ResourceTypes::ReadbackBuffer:
			DeallocateFromDeallocMemory(*bufferReadbackMemory, allocation);
			break;
		case ResourceTypes::UploadResource:
			DeallocateFromDeallocMemory(*uploadMemory, allocation);
			break;
		default:
			ThrowIfDebug(Exception::Exception{ "Resource type missing handling in dx12 resource factory deallocation" });				
		}
					
	}

		void ResourceFactoryDeallocatable::DeallocateFromDeallocMemory(AllocatableGpuMemory &fromMemory, ResourceAllocation &allocation)
		{
			reinterpret_cast<DeallocatableGpuMemory &>(fromMemory).Deallocate(allocation.allocation);
			CheckAndReleaseResourceRefs(allocation.resource);
							
		}

			void ResourceFactoryDeallocatable::CheckAndReleaseResourceRefs(DxPtr<ID3D12Resource> &resource)
			{
				const auto cutReferences{ resource.Reset() };
				
				if(cutReferences > 0)
				{
					ThrowIfDebug(Exception::Exception{ "Deallocated a resource allocation whose resource is still referenced" });
				}
		
			}


}