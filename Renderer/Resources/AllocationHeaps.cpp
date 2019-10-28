#include "DX12/HeapAllocation.hpp"
#include "DX12/Heap.hpp"
#include "Resources/AllocationHeaps.hpp"
#include <list>


namespace Renderer
{
	AllocationHeaps::AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes) :
		estimateBytesPerHeap{ initialHeapSizeInBytes }
	{
	}

	
	void AllocationHeaps::DeclareNewAllocatorID(unsigned ID)
	{
		if(heaps.count(ID) > 0)
		{
			return;
		}


	
		
	}

	
	void AllocationHeaps::RetireAllocatorID(unsigned ID)
	{
		auto foundPos{ heaps.find(ID) };

		//filter all heaps that are to small
		std::list<size_t> indicesToRemove{};		
		for(size_t index{}; index < foundPos->second.size(); ++index)
		{
			if(foundPos->second.at(index)->GetSizeInBytes() <= estimateBytesPerHeap)
			{
				indicesToRemove.emplace_back(index);
			}			
		}

		//remove all heaps that are to small
		for(auto &&index : indicesToRemove)
		{
			foundPos->second.erase(foundPos->second.begin() + index);
		}


		//merge remaining heaps with unused heaps
		heaps.at(0).insert(heaps.at(0).end(), foundPos->second.begin(), foundPos->second.end());

		//remove allocator data
		heaps.erase(foundPos);
		
	}

	
	RHA::DX12::HeapAllocation AllocationHeaps::Allocate(size_t sizeInBytes, unsigned allocatorID)
	{
	}

	
}
