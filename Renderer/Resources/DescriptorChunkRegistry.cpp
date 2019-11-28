#include "Resources/DescriptorChunkRegistry.hpp"


namespace Renderer
{
	DescriptorChunkRegistry::DescriptorChunkRegistry(const size_t descriptorCapacity)
	{
		freeChunks.emplace_back(0, descriptorCapacity);
		
	}

	DescriptorChunk DescriptorChunkRegistry::Allocate(const size_t descriptorCapacity)
	{
		auto freeChunk{ FindFreeChunk(descriptorCapacity) };

		if(freeChunk == freeChunks.end())
		{
			return { 0,0 };
			
		}

		DescriptorChunk allocation{};
		allocation.startIndex = freeChunk->startIndex;
		allocation.capacity = descriptorCapacity;

		freeChunk->startIndex += allocation.capacity;
		freeChunk->capacity -= allocation.capacity;

		if(freeChunk->capacity <= 0)
		{
			freeChunks.erase(freeChunk);
		}

		occupiedChunks.insert({ allocation.startIndex, allocation });

		return allocation;
		
		
	}

		DescriptorChunkRegistry::t_freeChunksItr DescriptorChunkRegistry::FindFreeChunk(const size_t descriptorCapacity)
		{		
			for(auto chunk{ freeChunks.begin() }; chunk != freeChunks.end(); ++chunk)
			{
				if(chunk->capacity >= descriptorCapacity)
				{
					return chunk;					
				}
				
			}

			return freeChunks.end();
		
		}



	void DescriptorChunkRegistry::Deallocate(const DescriptorChunk &chunk)
	{
		if(chunk.capacity == 0)
		{
			return;
		}
		
		auto allocation{ occupiedChunks.find(chunk.startIndex) };

		freeChunks.emplace_back(std::move(allocation->second));

		occupiedChunks.erase(allocation);
				
	}

	
}
