#include "Resources/Descriptor/ResourceViewFactoryImpl.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "StateSettings/FormatTargetsImpl.hpp"
#include "StateSettings/TargetHelpers.hpp"


namespace Renderer::DX12
{
	ResourceViewFactoryImpl::ResourceViewFactoryImpl(RHA::DX12::DeviceResources &resources, ResourceRegistry &registry) :
		registry{ &registry },
		memory{ &resources, 1000, 50 }
	{
	}


	
	void ResourceViewFactoryImpl::DeclareNewViewBlock(const ResourceHandle::t_hash forSignature, const size_t numViews, const size_t numSamplers)
	{
		this->forSignature = forSignature;
		currentAllocator = memory.GetDescriptorAllocator(numViews, numSamplers);
		currentAllocator.OpenNewTable();
		
	}


	
	void ResourceViewFactoryImpl::CreateShaderResourceView(const ResourceHandle::t_hash forResource, const size_t ordinal)
	{
		currentAllocator.CreateDefaultedSrv(registry->GetResource(forResource), registry->GetSignatureSrvOffset(forSignature, ordinal));
		
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
		currentAllocator.CreateSrvBuffer
		(
			registry->GetResource(forResource),
			registry->GetSignatureSrvOffset(forSignature, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		
	}


	
	void ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const size_t ordinal,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes,
		const t_format_target format
	)
	{
		currentAllocator.CreateSrvBufferFormatted
		(
			registry->GetResource(forResource),
			registry->GetSignatureSrvOffset(forSignature, ordinal),
			firstIndex,
			numElements,
			GetTargetValue<DXGI_FORMAT>(format)
		);
		
	}


	
	void ResourceViewFactoryImpl::CreateConstantBufferView
	(
		const ResourceHandle::t_hash forResource, 
		const size_t ordinal, 
		const size_t sizeInBytes
	)
	{
		currentAllocator.CreateCbv(registry->GetResource(forResource), registry->GetSignatureCbvOffset(forSignature, ordinal), sizeInBytes);
		
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
		currentAllocator.CreateUavBuffer
		(
			registry->GetResource(forResource),
			registry->GetSignatureUavOffset(forSignature, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		
	}


	
	void ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		ResourceHandle::t_hash forResource,
		size_t ordinal,
		size_t firstIndex,
		size_t numElements,		
		t_format_target format
	)
	{
		currentAllocator.CreateUavBufferFormatted
		(
			registry->GetResource(forResource),
			registry->GetSignatureUavOffset(forSignature, ordinal),
			firstIndex,
			numElements,
			GetTargetValue<DXGI_FORMAT>(format)
		);
		
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
		currentAllocator.CreateUavBufferWithCounter
		(
			registry->GetResource(forResource), 
			registry->GetResource(counterResource),
			registry->GetSignatureUavOffset(forSignature, ordinal),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		
	}


	
	ResourceHandle::t_hash ResourceViewFactoryImpl::GetCurrentViewBlock()
	{		
		return registry->Register(std::move(currentAllocator));
		
	}

	
}
