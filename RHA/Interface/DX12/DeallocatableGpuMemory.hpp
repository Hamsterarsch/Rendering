#pragma once
#include "DX12/AllocatableGpuMemory.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeallocatableGpuMemory : public AllocatableGpuMemory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DeallocatableGpuMemory)


			virtual void Deallocate(const HeapAllocation &allocation) = 0;
			
		};

		
	}

	
}

