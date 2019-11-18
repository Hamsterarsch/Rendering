#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/HeapAllocation.hpp"


namespace Renderer
{
	namespace DX12
	{
		class AllocatableGpuMemory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AllocatableGpuMemory)


			virtual RHA::DX12::HeapAllocation Allocate(size_t sizeInBytes) = 0;
			
		};

		
	}

	
}
