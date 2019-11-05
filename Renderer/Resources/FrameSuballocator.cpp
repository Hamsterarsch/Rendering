#include "Resources/FrameSuballocator.hpp"


namespace Renderer
{
	FrameSuballocator::FrameSuballocator(ResourceFactory *parent, const unsigned allocatorID) :
		parent{ parent },
		allocatorID{ allocatorID }
	{		
	}

	
}
