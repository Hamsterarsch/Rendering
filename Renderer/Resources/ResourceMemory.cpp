#include "DX12/Heap.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceMemory.hpp"


namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		HeapAllocation ResourceMemory::Allocate(const size_t sizeInBytes)
		{
			//find a memory block
						
			decltype(freeBlocks)::iterator targetBlock{};
			
			for(auto freeBlock{ freeBlocks.begin() }; freeBlock != freeBlocks.end(); ++freeBlock)
			{
				if(freeBlock->sizeInBytes >= sizeInBytes)
				{
					targetBlock = freeBlock;					
					break;
				}
			}

			//allocate a new heap bc there are no free blocks large enough
			if(targetBlock == freeBlocks.end()) 
			{
				auto heapSize{ initialHeapSize };
				if(heapSize <= sizeInBytes)
				{
					heapSize = sizeInBytes * 1.25;
				}
				
				memory.emplace_back
				(			
					Facade::MakeHeap
					(
						resources, 
						heapSize,
						D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
						heapFlags
					)
				);

				freeBlocks.emplace_back(memory.size() - 1, 0, heapSize);
				targetBlock = --freeBlocks.end();
			}

			//allocate
			memory.at(targetBlock->heapIndex)->


			//update free blocks

			//return allocation
			
			
		}

		
	}
	
	
}