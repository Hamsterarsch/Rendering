#pragma once
#include "AllocatableGpuMemory.hpp"


namespace Renderer
{
	namespace DX12
	{
		class DeallocatableGpuMemory : public AllocatableGpuMemory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DeallocatableGpuMemory)


			virtual void Deallocate(const RHA::DX12::HeapAllocation &allocation) = 0;
			
		};

		
	}

	
}

