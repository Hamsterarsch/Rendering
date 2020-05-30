#pragma once
#include "ResourceViewFactory.hpp"
#include "DescriptorMemory.hpp"
#include "ContainsReferences.hpp"
#include "ReferenceAwareDescriptorAllocator.hpp"


namespace Renderer::DX12
{
	class ResourceRegistry;

	
	class ResourceViewFactoryImpl final : public ResourceViewFactory
	{
		private: ResourceRegistry *registry;

		private: DescriptorMemory *memory;

		private: ReferenceAwareDescriptorAllocator currentAllocator;

		private: ResourceHandle::t_hash forSignature;
		
		
		
		public: ResourceViewFactoryImpl(RHA::DX12::DeviceResources &resources, ResourceRegistry &registry, DescriptorMemory &descriptors);

		
		public: void DeclareNewDescriptorBlock(ResourceHandle::t_hash forSignature, size_t numViews, size_t numSamplers) override;

		
		public: void CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t ordinal) override;

		public: void CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource,
			size_t ordinal,
			size_t firstIndex,
			size_t numElements, 
			size_t elementStrideInBytes
		) override;

		public: void CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource, 
			size_t ordinal,
			size_t firstIndex,
			size_t numElements,			
			Format format
		) override;

		public: void CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource, 
			size_t ordinal,
			Format format,
			uint16_t numMips,
			uint16_t mostDetailedMip
		) override;

		
		public: void CreateConstantBufferView(ResourceHandle::t_hash forResource, size_t ordinal, size_t sizeInBytes) override;

		
		public: void CreateSampler(ResourceHandle::t_hash forResource, size_t ordinal, const SamplerSpec &specification) override;

		
		public: void CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t ordinal,
			size_t firstIndex,
			size_t numElements,
			size_t elementStrideInBytes
		) override;

		public: void CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t ordinal,
			size_t firstIndex,
			size_t numElements,			
			Format format
		) override;

		public: void CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t ordinal,
			size_t firstIndex,
			size_t numElements,
			size_t elementStrideInBytes,
			ResourceHandle::t_hash counterResource
		) override;

		
		public: ResourceHandle::t_hash FinalizeDescriptorBlock() override;

	};
		
	
}

