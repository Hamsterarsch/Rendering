#include "Resources/FrameSuballocator.hpp"


namespace Renderer
{
	FrameSuballocator::FrameSuballocator(const ResourceAllocation &allocationRange, ResourceFactory *parent) :
		allocationRange{ allocationRange },
		parent{ parent }
	{		
	}

	
}
