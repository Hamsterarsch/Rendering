#include "Resources/ResourceFactory.hpp"
#include "Resources/FrameSuballocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	ResourceFactory::ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue) :
		queue{ queue },
		rescMemory{ resources, 1'310'720 }
	{
		uploadHeap = RHA::DX12::Facade::MakeUploadHeap(resources, 1'000'000);
		
	}

	FrameSuballocator ResourceFactory::MakeAllocatorForNewFrame()
	{		
		return FrameSuballocator{ this, rescMemory.MakeNewAllocatorID() };
		
	}

	
}
