#include "Resources/Descriptor/ResourceViewFactoryImpl.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "StateSettings/FormatTargetsImpl.hpp"
#include "DX12/DeviceResources.hpp"


namespace Renderer::DX12
{
	ResourceViewFactoryImpl::ResourceViewFactoryImpl(DeviceResources &resources, ResourceRegistry &registry, DescriptorMemory &descriptors)
		:
		resources{ &resources },
		registry{ &registry },
		memory{ &descriptors },
		currentTableIndex{ 0 },
		currentOrdinal{ 0 },
		currentOffsetIntoTable{ 0 },
		isBuildingSpecificDescriptor{ false }
	{
	}

	
	ResourceViewFactory &ResourceViewFactoryImpl::DeclareDescriptorBlock(const size_t numViews, const size_t numSamplers)
	{
		forSignature = 0;
		currentAllocator.allocator = memory->GetDescriptorAllocator(numViews, numSamplers);
		currentAllocator.allocator.OpenNewTable();
		currentOffsetIntoTable = 0;
		
		return *this;

	}



	ResourceViewFactory &ResourceViewFactoryImpl::DeclareSpecificDescriptorBlock(const ResourceHandle::t_hash forSignature, unsigned forTableIndex, const size_t numViews, const size_t numSamplers)
	{
		this->forSignature = forSignature;
		currentAllocator.allocator = memory->GetDescriptorAllocator(numViews, numSamplers);
		currentAllocator.allocator.OpenNewTable();
		currentTableIndex = forTableIndex;
		currentOrdinal = 0;
		isBuildingSpecificDescriptor = true;
		
		return *this;

	}

	ResourceViewFactory &ResourceViewFactoryImpl::DeclareOrdinal(const size_t ordinal)
	{
		currentOrdinal = ordinal;

		return *this;
		
	}



	ResourceViewFactory &ResourceViewFactoryImpl::CreateShaderResourceView(const ResourceHandle::t_hash forResource)
	{
		currentAllocator.allocator.CreateDefaultedSrv(registry->GetResource(forResource), GetTableOffset(&ResourceRegistry::GetSignatureSrvOffset));
		currentAllocator.AddReferenceTo(forResource);

		return *this;
		
	}

		size_t ResourceViewFactoryImpl::GetTableOffset(size_t(ResourceRegistry::* getter)(ResourceHandle::t_hash, unsigned, size_t))
		{
			Exception::ThrowIfDebug(isBuildingSpecificDescriptor && currentOrdinal == 0, {"Renderer::ResourceViewFactory: When creating descriptors for specific descriptor blocks you have to declare an ordinal."} );
		
			return isBuildingSpecificDescriptor ? (*registry.*getter)(forSignature, currentTableIndex, currentOrdinal) : currentOffsetIntoTable++;
		
		}



	ResourceViewFactory &ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes
	)
	{
		currentAllocator.allocator.CreateSrvBuffer
		(
			registry->GetResource(forResource),
			GetTableOffset(&ResourceRegistry::GetSignatureSrvOffset),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const size_t firstIndex,
		const size_t numElements,
		const Format format
	)
	{
		currentAllocator.allocator.CreateSrvBufferFormatted
		(
			registry->GetResource(forResource),
			GetTableOffset(&ResourceRegistry::GetSignatureSrvOffset),
			firstIndex,
			numElements,
			GetTargetValue(format)
		);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateShaderResourceView
	(
		const ResourceHandle::t_hash forResource,
		const Format format,
		const uint16_t numMips,
		const uint16_t mostDetailedMip
	)
	{
		currentAllocator.allocator.CreateSrvTex2D
		(
			registry->GetResource(forResource),
			GetTableOffset(&ResourceRegistry::GetSignatureSrvOffset),
			format,
			numMips,
			mostDetailedMip
		);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}



	ResourceViewFactory &ResourceViewFactoryImpl::CreateConstantBufferView
	(
		const ResourceHandle::t_hash forResource,
		const size_t sizeInBytes
	)
	{
		currentAllocator.allocator.CreateCbv(registry->GetResource(forResource), GetTableOffset(&ResourceRegistry::GetSignatureCbvOffset), sizeInBytes);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateSampler(const ResourceHandle::t_hash forResource, const SamplerSpec &specification)
	{
		throw;
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		const ResourceHandle::t_hash forResource,
		const size_t firstIndex,
		const size_t numElements,
		const size_t elementStrideInBytes
	)
	{
		currentAllocator.allocator.CreateUavBuffer
		(
			registry->GetResource(forResource),
			GetTableOffset(&ResourceRegistry::GetSignatureUavOffset),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		ResourceHandle::t_hash forResource,
		size_t firstIndex,
		size_t numElements,
		Format format
	)
	{
		currentAllocator.allocator.CreateUavBufferFormatted
		(
			registry->GetResource(forResource),
			GetTableOffset(&ResourceRegistry::GetSignatureUavOffset),
			firstIndex,
			numElements,
			GetTargetValue(format)
		);
		currentAllocator.AddReferenceTo(forResource);
		
		return *this;

	}


	
	ResourceViewFactory &ResourceViewFactoryImpl::CreateUnorderedAccessView
	(
		const ResourceHandle::t_hash forResource,
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
			GetTableOffset(&ResourceRegistry::GetSignatureUavOffset),
			firstIndex,
			numElements,
			elementStrideInBytes
		);
		currentAllocator.AddReferenceTo(forResource);
		currentAllocator.AddReferenceTo(counterResource);
		
		return *this;

	}


	
	ResourceHandle::t_hash ResourceViewFactoryImpl::FinalizeDescriptorBlock()
	{
		isBuildingSpecificDescriptor = false;		
		return registry->Register(std::move(currentAllocator));
		
	}


	
	ResourceHandle::t_hash ResourceViewFactoryImpl::MakeDepthTextureView(const ResourceHandle::t_hash forResource)
	{
		Exception::ThrowIfDebug(ResourceHandle::GetResourceType(forResource) != ResourceHandle::t_resourceTypes::DepthTexture, {"ResourceViewFactory: You can only create depth texture views for depth textures"});
		
		auto targetDescriptor{ memory->AllocateDepthStencilDescriptor() };		
		resources->GetDevice()->CreateDepthStencilView(registry->GetResource(forResource), nullptr, targetDescriptor);

		return registry->Register(RawDescriptorReference{ *memory, targetDescriptor, forResource} );
		
	}
	
	
}
