#include "DX12/Heap.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Resources/DynamicHeapSet.hpp"


namespace Renderer
{	
	using namespace RHA::DX12;
	
	DynamicHeapSet::DynamicHeapSet(RHA::DX12::DeviceResources *resources, const size_t initialHeapSizeInBytes, D3D12_HEAP_FLAGS flags) :
		estimateBytesPerHeap{ initialHeapSizeInBytes },
		allowedEstimateDeviation{ 0.2 },
		resources{ resources },
		heapFlags{ flags }
	{
		heaps.emplace_back(Facade::MakeHeap(resources, estimateBytesPerHeap, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, flags));
	}
	
	
	
	HeapAllocation DynamicHeapSet::Allocate(size_t sizeInBytes)
	{
		//get a target heap
		
		//try to find an existing one with enough space
		t_item *targetHeap{ nullptr };
		for(auto &heap : heaps)
		{
			if(heap->HasCapacityForAllocation(sizeInBytes))
			{
				targetHeap = &heap;
				break;
			}
		}

		//allocate a new heap bc there is none with the required size
		if(targetHeap == nullptr) 
		{
			//update the heap size estimate
			estimateBytesPerHeap *= (1+allowedEstimateDeviation);				
			
			heaps.emplace_back
			(
				Facade::MakeHeap
				(
					resources, 
					max(estimateBytesPerHeap, sizeInBytes),
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
					heapFlags
				)
			);

			targetHeap = &heaps.back();			
		}
		
		//make allocation
		return (*targetHeap)->Allocate(sizeInBytes);
		
	}

	
}
