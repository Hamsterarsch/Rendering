#include "Resources/ResourceFactory.hpp"
#include "Resources/FrameSuballocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	ResourceFactory::ResourceFactory(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue) :
		queue{ queue },
		bufferRescMemory{ resources, 1'310'720, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS },
		resources{ resources }
	{
	}

	FrameSuballocator ResourceFactory::MakeAllocatorForNewFrame()
	{		
		return FrameSuballocator{ resources, queue, this, bufferRescMemory.MakeNewAllocatorID() };
		
	}

	RHA::DX12::HeapAllocation ResourceFactory::MakeRawAllocationForBuffer(const size_t sizeInBytes, const unsigned allocatorID)
	{		
		return bufferRescMemory.Allocate(sizeInBytes, allocatorID);
		
	}

	
}
