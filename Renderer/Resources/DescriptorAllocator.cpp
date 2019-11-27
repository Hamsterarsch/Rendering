#include "Resources/DescriptorAllocator.hpp"


namespace Renderer
{
	namespace DX12
	{
		DescriptorAllocator::DescriptorAllocator(const DescriptorChunk &viewChunk, const DescriptorChunk &samplerChunk) :
			view{ viewChunk, 0 },
			sampler{ samplerChunk, 0 }		
		{
		}

		
	}

	
}