#pragma once
#include "ResourceViewFactory.hpp"
#include "DescriptorMemory.hpp"


namespace Renderer::DX12
{
	class ResourceRegistry;

	class ResourceViewFactoryImpl final : public ResourceViewFactory
	{
		private: ResourceRegistry *registry;

		private: DescriptorMemory memory;

		private: DescriptorAllocator currentAllocator;

		private: ResourceHandle::t_hash forSignature;
		
		
		
		public: ResourceViewFactoryImpl(RHA::DX12::DeviceResources &resources, ResourceRegistry &registry);

		
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
			size_t elementStrideInBytes,
			t_format_target format
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
			t_format_target format
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

