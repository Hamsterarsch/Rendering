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


			virtual HeapAllocation Allocate(size_t sizeInBytes) = 0;
			
		};

		
	}

	
}
