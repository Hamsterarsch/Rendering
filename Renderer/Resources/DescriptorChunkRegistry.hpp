#pragma once
#include <list>
#include <unordered_map>
#include "Resources/DescriptorChunk.hpp"


namespace Renderer
{		
	class DescriptorChunkRegistry
	{
		private: std::list<DescriptorChunk> freeChunks;
		
		private: std::unordered_map<size_t, DescriptorChunk> occupiedChunks;

		private: using t_freeChunksItr = decltype(freeChunks)::iterator;

		
		public: DescriptorChunkRegistry(size_t descriptorCapacity);
						
		public: DescriptorChunk Allocate(size_t descriptorCapacity);

			private: t_freeChunksItr FindFreeChunk(size_t descriptorCapacity);


		public: void Deallocate(const DescriptorChunk &chunk);
				
	};

	
}
