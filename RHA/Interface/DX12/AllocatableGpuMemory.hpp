#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/HeapAllocation.hpp"


namespace RHA
{
	namespace DX12
	{
		class AllocatableGpuMemory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AllocatableGpuMemory)


			virtual HeapAllocation Allocate(size_t sizeInBytes) = 0;
			
		};

		
	}

	
}
