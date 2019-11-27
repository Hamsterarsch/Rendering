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
			initialHeapSizeInBytes{ RHA::Utility::IncreaseValueToAlignment(initialHeapSizeInBytes, alignment) },
			alignment{ alignment },
			heapFlags{ heapFlags }		
		{
			MakeNewFreeBlock(initialHeapSizeInBytes);
			
		}

			ResourceMemory::t_freeBlocksItr ResourceMemory::MakeNewFreeBlock(const size_t sizeInBytes)
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

				const auto newHeapIndex{ memory.size()-1 };
				heapIndexMap.insert( {memory.back().heap->GetHeap().Get(), newHeapIndex} );

				freeBlocks.emplace_back( MemoryInfo{ newHeapIndex, 0, sizeInBytes } );
				return --freeBlocks.end();
			
			}



		HeapAllocation ResourceMemory::Allocate(const size_t sizeInBytes)
		{			
			auto targetBlock{ FindFreeBlock(sizeInBytes) };						
			if(targetBlock == freeBlocks.end()) 
			{			
				targetBlock = MakeNewFreeBlock(GetBlockSizeForNewAllocation(sizeInBytes));
			}

			const auto allocation{ AllocateFromBlock(targetBlock, sizeInBytes) };
			RegisterAllocationAsOccupiedBlock(targetBlock->heapIndex, allocation);			
			AdjustFreeBlockDataAfterAllocation(targetBlock, allocation);
			
			return allocation;
			
		}

			ResourceMemory::t_freeBlocksItr ResourceMemory::FindFreeBlock(const size_t sizeInBytes)
			{
				auto foundBlock{ freeBlocks.end() };
			
				for(auto currentBlock{ freeBlocks.begin() }; currentBlock != freeBlocks.end(); ++currentBlock)
				{
					if(currentBlock->sizeInBytes >= sizeInBytes)
					{
						foundBlock = currentBlock;					
						break;
					}
				}

				return foundBlock;
			
			}

			size_t ResourceMemory::GetBlockSizeForNewAllocation(const size_t allocationSizeInBytes) const
			{
				auto newBlockSize{ initialHeapSizeInBytes };
				if(newBlockSize <= allocationSizeInBytes)
				{
					newBlockSize = RHA::Utility::IncreaseValueToAlignment(allocationSizeInBytes * 1.25, alignment);
				}

				return newBlockSize;
			
			}

			HeapAllocation ResourceMemory::AllocateFromBlock(const t_freeBlocksItr &block, size_t allocationSizeInBytes)
			{
				HeapAllocation allocation{};
				allocation.heap = memory.at(block->heapIndex).heap->GetHeap().Get();
				allocation.offsetToAllocation = block->offsetToAllocation;
				allocation.allocationSize = min( GetAlignedAllocationSizeForBlock(block, allocationSizeInBytes), block->sizeInBytes );

				return allocation;
			
			}

				size_t ResourceMemory::GetAlignedAllocationSizeForBlock
				(
					const decltype(freeBlocks)::iterator &block,
					const size_t allocationSizeInBytes
				)
				{
					const auto offsetToAllocationEnd{ block->offsetToAllocation + allocationSizeInBytes };
					return RHA::Utility::IncreaseValueToAlignment(offsetToAllocationEnd, alignment) - block->offsetToAllocation;
			
				}

			void ResourceMemory::RegisterAllocationAsOccupiedBlock(const size_t targetHeapIndex, const HeapAllocation &allocation)
			{
				memory.at(targetHeapIndex).occupiedBlocks.insert
				({
					allocation.offsetToAllocation,
					MemoryInfo{ targetHeapIndex, allocation.offsetToAllocation, allocation.allocationSize }
				});
			
			}

			void ResourceMemory::AdjustFreeBlockDataAfterAllocation
			(
				decltype(freeBlocks)::iterator &block,
				const HeapAllocation &allocation
			)
			{
				block->offsetToAllocation += allocation.allocationSize;
				block->sizeInBytes -= allocation.allocationSize;
								
				if(block->sizeInBytes <= 0)
				{
					freeBlocks.erase(block);
				}
			
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