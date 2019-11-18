#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/HeapAllocation.hpp"
#include "DX12/AllocatableGpuMemory.hpp"
#include "DxPtrTypes.hpp"


struct ID3D12Heap;

namespace RHA
{
	namespace DX12
	{
		class Heap : public AllocatableGpuMemory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Heap)

			
			public: virtual size_t GetSizeInBytes() const = 0;			
			
			public: virtual bool HasCapacityForAllocation(size_t allocationSizeInBytes) const = 0;
					
			public: virtual void Reset() = 0;

			public: virtual DxPtr<ID3D12Heap> GetHeap() = 0;
			
		};

		
	}

	
}
