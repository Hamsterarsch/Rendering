#include "DX12/Heap.hpp"
#include "DX12/Facade.hpp"
#include "Resources/ResourceMemory.hpp"
#include "Utility/Alignment.hpp"


namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;

		ResourceMemory::ResourceMemory(DeviceResources *resources, const size_t initialHeapSizeInBytes, const size_t alignment, const D3D12_HEAP_FLAGS heapFlags) :
			resources{ resources },
			initialHeapSize{ RHA::Utility::IncreaseValueToAlignment(initialHeapSizeInBytes, alignment) },
			alignment{ alignment },
			heapFlags{ heapFlags }		
		{
			MakeNewFreeBlock(initialHeapSize);
			
		}

			decltype(ResourceMemory::freeBlocks)::iterator ResourceMemory::MakeNewFreeBlock(const size_t sizeInBytes)
			{
				memory.emplace_back
				(
					HeapInfo
					{
						Facade::MakeHeap
						(
							resources, 
							sizeInBytes,
							alignment,
							heapFlags
						),
						{}
					}
				);
				
				heapIndexMap.insert( {memory.back().heap->GetHeap().Get(), memory.size() - 1} );

				freeBlocks.emplace_back( MemoryInfo{ memory.size() - 1, 0, sizeInBytes } );
				return --freeBlocks.end();
			
			}


		
		HeapAllocation ResourceMemory::Allocate(const size_t sizeInBytes)
		{
			//find a memory block						
			auto targetBlock{ freeBlocks.end() };
			
			for(auto freeBlock{ freeBlocks.begin() }; freeBlock != freeBlocks.end(); ++freeBlock)
			{
				if(freeBlock->sizeInBytes >= sizeInBytes)
				{
					targetBlock = freeBlock;					
					break;
				}
			}

			//allocate a new block bc there are no free blocks large enough
			if(targetBlock == freeBlocks.end()) 
			{
				auto newBlockSize{ initialHeapSize };
				if(newBlockSize <= sizeInBytes)
				{
					newBlockSize = RHA::Utility::IncreaseValueToAlignment(sizeInBytes * 1.25, alignment);
				}
								
				targetBlock = MakeNewFreeBlock(newBlockSize);
			}

			//"allocate" the block's memory
			HeapAllocation alloc{};
			alloc.heap = memory.at(targetBlock->heapIndex).heap->GetHeap().Get();
			alloc.offsetToAllocation = targetBlock->offsetToAllocation;
			alloc.allocationSize = min(RHA::Utility::IncreaseValueToAlignment(targetBlock->offsetToAllocation + sizeInBytes, alignment) - targetBlock->offsetToAllocation, targetBlock->sizeInBytes);
			
			//update the old block
			targetBlock->offsetToAllocation += alloc.allocationSize;
			targetBlock->sizeInBytes -= alloc.allocationSize;

			if(targetBlock->sizeInBytes <= 0)
			{
				freeBlocks.erase(targetBlock);
			}

			//add an occupied block
			memory.at(targetBlock->heapIndex).occupiedBlocks.insert( {alloc.offsetToAllocation, MemoryInfo{ targetBlock->heapIndex, alloc.offsetToAllocation, alloc.allocationSize} } );
			
			return alloc;
			
		}


				
		void ResourceMemory::Deallocate(const HeapAllocation &allocation)
		{
			//find data
			const auto targetIndex{ heapIndexMap.at(allocation.heap) };
			auto occupiedBlock{ memory.at(targetIndex).occupiedBlocks.find(allocation.offsetToAllocation) };

			if(occupiedBlock == memory.at(targetIndex).occupiedBlocks.end())
			{
				return;
			}

			//add now unused block to free blocks
			freeBlocks.emplace_back( std::move(occupiedBlock->second) );

			memory.at(targetIndex).occupiedBlocks.erase(occupiedBlock);
						
		}

		
	}
	
	
}