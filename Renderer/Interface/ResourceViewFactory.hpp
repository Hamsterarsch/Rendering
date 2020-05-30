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


		public: virtual void DeclareNewDescriptorBlock(ResourceHandle::t_hash forSignature, size_t numViews, size_t numSamplers) = 0;
		
		
		public: virtual void CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t ordinal) = 0;

		public: virtual void CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t ordinal, size_t firstIndex, size_t numElements, size_t elementStrideInBytes) = 0;

		public: virtual void CreateShaderResourceView(ResourceHandle::t_hash forResource, size_t ordinal, size_t firstIndex, size_t numElements, Format format) = 0;

		public: virtual void CreateShaderResourceView
		(
			ResourceHandle::t_hash forResource, 
			size_t ordinal,
			Format format,
			uint16_t numMips,
			uint16_t mostDetailedMip
		) = 0;
		

		public: virtual void CreateConstantBufferView(ResourceHandle::t_hash forResource, size_t ordinal, size_t sizeInBytes) = 0;


		public: virtual void CreateSampler(ResourceHandle::t_hash forResource, size_t ordinal, const SamplerSpec &specification) = 0;


		public: virtual void CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t ordinal, size_t firstIndex, size_t numElements, size_t elementStrideInBytes) = 0;

		public: virtual void CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t ordinal, size_t firstIndex, size_t numElements, Format format) = 0;

		public: virtual void CreateUnorderedAccessView(ResourceHandle::t_hash forResource, size_t ordinal, size_t firstIndex, size_t numElements, size_t elementStrideInBytes, ResourceHandle::t_hash counterResource) = 0;


		public: virtual ResourceHandle::t_hash FinalizeDescriptorBlock() = 0;
		
	};
	
	
}