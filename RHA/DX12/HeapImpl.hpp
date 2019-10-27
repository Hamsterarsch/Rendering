#pragma once
#include "DX12/Heap.hpp"
#include "DX12/DeviceResources.hpp"
#include "DxPtrTypes.hpp"



namespace RHA
{
	namespace DX12
	{		
		class HeapImpl : public Heap
		{
			private: DxPtr<ID3D12Heap> heap;

			private: size_t sizeInBytes;

			private: size_t offsetToFreeRegion;

			private: size_t alignment;

			

			public: HeapImpl(DeviceResources *resources, size_t sizeInBytes, size_t alignment);

				private: size_t IncreaseValueToAlignment(size_t value);
			
				private: void CheckHeapCreation(HRESULT result) const;
			
				private: bool AlignmentIsInvalid() const;

			
			public: bool HasCapacityForAllocation(size_t allocationSizeInBytes) const;

			
			public: virtual struct HeapAllocation Allocate(size_t sizeInBytes) override;

				private: void CheckAllocationSize(size_t allocationSizeInBytes) const;
						
				private: void OnAllocationMade(size_t sizeInBytes);
			
			
						
		};

		
	}
	
	
}
