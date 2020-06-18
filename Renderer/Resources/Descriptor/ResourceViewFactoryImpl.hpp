#pragma once
#include "ResourceViewFactory.hpp"
#include "DescriptorMemory.hpp"
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

		private: unsigned currentTableIndex;

		private: size_t currentOrdinal;

		private: size_t currentOffsetIntoTable;

		private: bool isBuildingSpecificDescriptor;

		
		
		public: ResourceViewFactoryImpl(ResourceRegistry &registry, DescriptorMemory &descriptors);

		
		public: ResourceViewFactory &DeclareDescriptorBlock(size_t numViews, size_t numSamplers) override;
		
		public: ResourceViewFactory &DeclareSpecificDescriptorBlock(ResourceHandle::t_hash forSignature, unsigned forTableIndex, size_t numViews, size_t numSamplers) override;
		
		public: ResourceViewFactory &DeclareOrdinal(size_t ordinal) override;

		
		public: ResourceViewFactory &CreateShaderResourceView(ResourceHandle::t_hash forResource) override;

			private: size_t GetTableOffset(size_t(ResourceRegistry:: *getter)(ResourceHandle::t_hash, unsigned, size_t))
		
		public: ResourceViewFactory &CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource,
			size_t firstIndex,
			size_t numElements,
			size_t elementStrideInBytes
		) override;

		public: ResourceViewFactory &CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource,
			size_t firstIndex,
			size_t numElements,
			Format format
		) override;

		public: ResourceViewFactory &CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource,
			Format format,
			uint16_t numMips,
			uint16_t mostDetailedMip
		) override;

		
		public: ResourceViewFactory &CreateConstantBufferView(ResourceHandle::t_hash forResource, size_t sizeInBytes) override;

		
		public: ResourceViewFactory &CreateSampler(ResourceHandle::t_hash forResource, const SamplerSpec &specification) override;

		
		public: ResourceViewFactory &CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t firstIndex,
			size_t numElements,
			size_t elementStrideInBytes
		) override;

		public: ResourceViewFactory &CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t firstIndex,
			size_t numElements,
			Format format
		) override;

		public: ResourceViewFactory &CreateUnorderedAccessView
		(
			ResourceHandle::t_hash forResource,
			size_t firstIndex,
			size_t numElements,
			size_t elementStrideInBytes,
			ResourceHandle::t_hash counterResource
		) override;

		public: ResourceHandle::t_hash FinalizeDescriptorBlock() override;


	};
		
	
}

