#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/HeapAllocation.hpp"


namespace RHA
{
	namespace DX12
	{
		class Heap
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Heap)

			
			public: virtual size_t GetSizeInBytes() const = 0;			
			
			public: virtual bool HasCapacityForAllocation(size_t allocationSizeInBytes) const = 0;

			public: virtual HeapAllocation Allocate(size_t sizeInBytes) = 0;

			public: virtual void Reset() = 0;	
			
		};

		
	}

	
}
