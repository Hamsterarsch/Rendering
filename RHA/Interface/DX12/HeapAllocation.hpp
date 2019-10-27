#pragma once
#include "RHAExportHelper.hpp"


struct ID3D12Heap;

namespace RHA
{
	namespace DX12
	{
		struct RHA_DLLSPEC HeapAllocation
		{
			ID3D12Heap *heap;
			size_t offsetToAllocation;
			size_t allocationSize;
			
		};

		
	}

	
}
