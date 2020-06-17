#include "Resources/Descriptor/ResourceViewFactoryImpl.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "StateSettings/FormatTargetsImpl.hpp"


namespace Renderer::DX12
{
	ResourceViewFactoryImpl::ResourceViewFactoryImpl(ResourceRegistry &registry, DescriptorMemory &descriptors) :
		registry{ &registry },
		memory{ &descriptors },
		currentTableIndex{ 0 }
	{
	}


	
	void ResourceViewFactoryImpl::DeclareNewDescriptorBlock(const ResourceHandle::t_hash forSignature, unsigned forTableIndex, const size_t numViews, const size_t numSamplers)
	{
		this->forSignature = forSignature;
		currentAllocator.allocator = memory->GetDescriptorAllocator(numViews, numSamplers);
		currentAllocator.allocator.OpenNewTable();
		currentTableIndex = forTableIndex;
		
	}


	
	void ResourceViewFactoryImpl::CreateShaderResourceView(const ResourceHandle::t_hash forResource, const size_t ordinal)
	{
		currentAllocator.allocator.CreateDefaultedSrv(registry->GetResource(forResource), registry->GetSignatureSrvOffset(forSignature, currentTableIndex, ordinal));
		currentAllocator.AddReferenceTo(forResource);
		
	}

	void ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const size_t ordinal,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes
	)
	{
		currentAllocator.allocator.CreateSrvBuffer
		(
			registry->GetResource(forResource),
			registry->GetSignatureSrvOffset(forSignature, currentTableIndex, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		
	}


	
	void ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const size_t ordinal,
		const size_t firstIndex,
		const size_t numElements,		
		const Format format
	)
	{
		currentAllocator.allocator.CreateSrvBufferFormatted
		(
			registry->GetResource(forResource),
			registry->GetSignatureSrvOffset(forSignature, currentTableIndex, ordinal),
			firstIndex,
			numElements,
			GetTargetValue(format)
		);
		currentAllocator.AddReferenceTo(forResource);
		
	}


	
	void ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource, 
		const size_t ordinal,
		const Format format,
		const uint16_t numMips,
		const uint16_t mostDetailedMip
	)
	{
		currentAllocator.allocator.CreateSrvTex2D
		(
			registry->GetResource(forResource),
			registry->GetSignatureSrvOffset(forSignature, currentTableIndex, ordinal),
			format,
			numMips,
			mostDetailedMip
		);
		currentAllocator.AddReferenceTo(forResource);
		
	}



	void ResourceViewFactoryImpl::CreateConstantBufferView
	(
		const ResourceHandle::t_hash forResource, 
		const size_t ordinal, 
		const size_t sizeInBytes
	)
	{
		currentAllocator.allocator.CreateCbv(registry->GetResource(forResource), registry->GetSignatureCbvOffset(forSignature, currentTableIndex, ordinal), sizeInBytes);
		currentAllocator.AddReferenceTo(forResource);
		
	}


	
	void ResourceViewFactoryImpl::CreateSampler(const ResourceHandle::t_hash forResource, const size_t ordinal, const SamplerSpec &specification)
	{
		throw;
		
	}


	
	void ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		const ResourceHandle::t_hash forResource,
		const size_t ordinal,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes
	)
	{
		currentAllocator.allocator.CreateUavBuffer
		(
			registry->GetResource(forResource),
			registry->GetSignatureUavOffset(forSignature, currentTableIndex, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		
	}


	
	void ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		ResourceHandle::t_hash forResource,
		size_t ordinal,
		size_t firstIndex,
		size_t numElements,		
		Format format
	)
	{
		currentAllocator.allocator.CreateUavBufferFormatted
		(
			registry->GetResource(forResource),
			registry->GetSignatureUavOffset(forSignature, currentTableIndex, ordinal),
			firstIndex,
			numElements,
			GetTargetValue(format)
		);
		currentAllocator.AddReferenceTo(forResource);
		
	}


	
	void ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		const ResourceHandle::t_hash forResource,
		const size_t ordinal,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes,
		const ResourceHandle::t_hash counterResource
	)
	{
		currentAllocator.allocator.CreateUavBufferWithCounter
		(
			registry->GetResource(forResource), 
			registry->GetResource(counterResource),
			registry->GetSignatureUavOffset(forSignature, currentTableIndex, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		currentAllocator.AddReferenceTo(counterResource);
		
	}


	
	ResourceHandle::t_hash ResourceViewFactoryImpl::FinalizeDescriptorBlock()
	{		
		return registry->Register(std::move(currentAllocator));
		
	}

	
}
