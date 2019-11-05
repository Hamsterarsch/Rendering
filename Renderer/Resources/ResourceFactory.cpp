#include "Resources/ResourceFactory.hpp"
#include "Resources/FrameSuballocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	ResourceFactory::ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue) :
		queue{ queue },
		rescMemory{ resources, 1'310'720 },
		resources{ resources }
	{
	}

	FrameSuballocator ResourceFactory::MakeAllocatorForNewFrame()
	{		
		return FrameSuballocator{ resources, queue, this, rescMemory.MakeNewAllocatorID() };
		
	}

	RHA::DX12::HeapAllocation ResourceFactory::MakeRawAllocation(const size_t sizeInBytes, const unsigned allocatorID)
	{		
		return rescMemory.Allocate(sizeInBytes, allocatorID);
		
	}

	
}
