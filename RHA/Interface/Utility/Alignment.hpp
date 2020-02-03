#pragma once


namespace RHA
{
	namespace Utility
	{
		inline size_t IncreaseValueToAlignment(const size_t value, const size_t alignment)
		{
			return (value + alignment-1) & ~(alignment-1);
		
		}

		inline bool AlignmentIsInvalid(const size_t alignment)
		{
			return alignment == 0	||	 alignment & alignment-1;
			
		}

		
	}
	
	
}
