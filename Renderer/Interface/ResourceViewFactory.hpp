#pragma once
#include <cstdint>
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"
#include "StateSettings/FormatTargets.hpp"


namespace Renderer
{
	struct SamplerSpec;

	class ResourceViewFactory
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ResourceViewFactory)


		public: virtual ResourceViewFactory &DeclareDescriptorBlock(size_t numViews, size_t numSamplers) = 0;
		
		public: virtual ResourceViewFactory &DeclareSpecificDescriptorBlock(ResourceHandle::t_hash forSignature, unsigned forTableIndex, size_t numViews, size_t numSamplers) = 0;

		public: virtual ResourceViewFactory &DeclareOrdinal(size_t ordinal) = 0;
		
		
		public: virtual ResourceViewFactory &CreateShaderResourceView(ResourceHandle::t_hash forResource) = 0;

		public: virtual ResourceViewFactory &CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t firstIndex, size_t numElements, size_t elementStrideInBytes) = 0;

		public: virtual ResourceViewFactory &CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t firstIndex, size_t numElements, Format format) = 0;

		public: virtual ResourceViewFactory &CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource,
			Format format,
			uint16_t numMips,
			uint16_t mostDetailedMip
		) = 0;
		

		public: virtual ResourceViewFactory &CreateConstantBufferView(ResourceHandle::t_hash forResource, size_t sizeInBytes) = 0;


		public: virtual ResourceViewFactory &CreateSampler(ResourceHandle::t_hash forResource, const SamplerSpec &specification) = 0;


		public: virtual ResourceViewFactory &CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t firstIndex, size_t numElements, size_t elementStrideInBytes) = 0;

		public: virtual ResourceViewFactory &CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t firstIndex, size_t numElements, Format format) = 0;

		public: virtual ResourceViewFactory &CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t firstIndex, size_t numElements, size_t elementStrideInBytes, ResourceHandle::t_hash counterResource) = 0;
						

		public: virtual ResourceHandle::t_hash FinalizeDescriptorBlock() = 0;

		public: virtual ResourceHandle::t_hash MakeDepthTextureView(ResourceHandle::t_hash forResource) = 0;
		
	};
	
	
}
