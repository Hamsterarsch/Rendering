#pragma once


namespace Renderer
{
	struct DescriptorChunk
	{
		size_t startIndex;
		size_t capacity;

		DescriptorChunk() :
			startIndex{ 0 },
			capacity{ 0 }
		{}

		DescriptorChunk(size_t startIndex, size_t capacity) :
			startIndex{ startIndex },
			capacity{ capacity }
		{}
		
	};

	
}
