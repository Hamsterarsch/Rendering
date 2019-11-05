#include <list>
#include <algorithm>
#include "DX12/HeapAllocation.hpp"
#include "DX12/Heap.hpp"
#include "DX12/Facade.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Resources/AllocationHeaps.hpp"

#undef max

namespace Renderer
{	
	AllocationHeaps::AllocationHeaps(RHA::DX12::DeviceResources *resources, const size_t initialHeapSizeInBytes, D3D12_HEAP_FLAGS flags) :
		estimateBytesPerHeap{ initialHeapSizeInBytes },
		allowedEstimateDeviation{ 0.2 },
		newestDeclaredAllocatorID{ 0 },
		resources{ resources },
		heapFlags{ flags }
	{
		heaps.insert( { 0, std::list<UniquePtr<RHA::DX12::Heap>>() } );
		
	}

	
	unsigned AllocationHeaps::MakeNewAllocatorID()
	{
		//handle overflow
		if(newestDeclaredAllocatorID == std::numeric_limits<decltype(newestDeclaredAllocatorID)>::max())
		{
			newestDeclaredAllocatorID = 1;			
		}
		else
		{
			++newestDeclaredAllocatorID;			
		}

		if(heaps.count(newestDeclaredAllocatorID) > 0)
		{
			throw Exception::Exception{ "DX12::AllocationHeaps; Tried to make a new allocator id that is still occupied. Please retire more existing ids" };
		}
		
		//try to get an existing unused heap
		if(heaps.at(0).size() > 0)
		{
			heaps.insert({ newestDeclaredAllocatorID, std::list<UniquePtr<RHA::DX12::Heap>>() });
			heaps.at(newestDeclaredAllocatorID).emplace_back( std::move(heaps.at(0).back()) );
			heaps.at(0).pop_back();
			
			return newestDeclaredAllocatorID;			
		}

		//construct a new heap
		heaps.insert({ newestDeclaredAllocatorID, std::list<UniquePtr<RHA::DX12::Heap>>() });
		heaps.at(newestDeclaredAllocatorID).emplace_back(RHA::DX12::Facade::MakeHeap(resources, estimateBytesPerHeap, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, heapFlags));
		return newestDeclaredAllocatorID;
		
	}

	
	void AllocationHeaps::RetireAllocatorID(const unsigned ID)
	{
		if(ID == 0 || heaps.count(ID) == 0)
		{
			throw Exception::Exception{ "DX12::AllocationHeaps; Tried to retire an allocator id that does not exist" };
		}
		
		auto foundPos{ heaps.find(ID) };

		--foundPos->second.begin();
		
		//filter all heaps that are to small or too large
		std::list<size_t> indicesToRemove{};		
		for(auto itr{ foundPos->second.begin() }; itr != foundPos->second.end(); ++itr)
		{
			if(std::abs((*itr)->GetSizeInBytes() / static_cast<float>(estimateBytesPerHeap) -1) > allowedEstimateDeviation)
			{
				const auto toRemove = itr;
				--itr;

				foundPos->second.erase(toRemove);				
			}			
		}

		//merge remaining heaps with unused heaps
		heaps.at(0).splice(heaps.at(0).end(), std::move(foundPos->second));

		//remove allocator data
		heaps.erase(foundPos);
		
	}

	
	RHA::DX12::HeapAllocation AllocationHeaps::Allocate(size_t sizeInBytes, unsigned allocatorID)
	{
		//get a target heap
		if(heaps.count(allocatorID) == 0)
		{
			throw Exception::Exception{ "DX12::AllocationHeaps; Tried to make an allocation using an unregistered allocator id" };
		}

		//try to find an existing one with enough space
		UniquePtr<RHA::DX12::Heap> *targetHeap{ nullptr };
		for(auto &heap : heaps.at(allocatorID))
		{
			if(heap->HasCapacityForAllocation(sizeInBytes))
			{
				targetHeap = &heap;
				break;
			}
		}

		//allocate a new heap
		if(targetHeap == nullptr) 
		{
			//update the heap size estimate if this allocation is for the newest allocator
			if(allocatorID == newestDeclaredAllocatorID)
			{
				 estimateBytesPerHeap *= (1+allowedEstimateDeviation);				
			}

			heaps.at(allocatorID).emplace_back
			(
				RHA::DX12::Facade::MakeHeap
				(
					resources, 
					std::max(estimateBytesPerHeap, sizeInBytes),
					D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
					heapFlags
				)
			);

			targetHeap = &heaps.at(allocatorID).back();
			
		}
		
		//make allocation
		return (*targetHeap)->Allocate(sizeInBytes);
		
	}

	
}
