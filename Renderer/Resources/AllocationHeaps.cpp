#include "DX12/HeapAllocation.hpp"
#include "DX12/Heap.hpp"
#include "Resources/AllocationHeaps.hpp"
#include <list>
#include "DX12/Facade.hpp"


namespace Renderer
{
	AllocationHeaps::AllocationHeaps(RHA::DX12::DeviceResources *resources, size_t initialHeapSizeInBytes) :
		estimateBytesPerHeap{ initialHeapSizeInBytes },
		resources{ resources }
	{
		heaps.insert( { 0,{} } );
		
	}

	
	void AllocationHeaps::DeclareNewAllocatorID(unsigned ID)
	{
		if(heaps.count(ID) > 0 || ID == 0)
		{
			return;
		}

		//try to get an existing unused heap
		if(heaps.at(0).size() > 0)
		{
			heaps.insert({ ID, { std::move(heaps.at(0).back()) } });
			heaps.at(0).erase(heaps.at(0).end() -1);
			return;
			
		}

		//construct a new heap
		heaps.insert({ ID, {RHA::DX12::Facade::MakeHeap(resources, estimateBytesPerHeap, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)} });
		
	}

	
	void AllocationHeaps::RetireAllocatorID(unsigned ID)
	{
		if(ID == 0)
		{
			return;
		}
		
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
