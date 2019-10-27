#include "Resources/FrameSuballocator.hpp"


namespace Renderer
{
	FrameSuballocator::FrameSuballocator(const ResourceAllocation &allocationRange, ResourceFactory *parent, const unsigned allocatorID) :
		allocationRange{ allocationRange },
		parent{ parent },
		allocatorID{ allocatorID }
	{		
	}

	
}
