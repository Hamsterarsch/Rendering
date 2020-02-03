#pragma once
#include "DX12/Heap.hpp"
#include "DX12/DeviceResources.hpp"




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

			

			public: HeapImpl(DeviceResources *resources, size_t sizeInBytes, size_t alignment, D3D12_HEAP_FLAGS flags);

				private: static void CheckHeapCreation(HRESULT result);
			
						
			public: virtual inline size_t GetSizeInBytes() const override { return sizeInBytes; }
											
			public: virtual bool HasCapacityForAllocation(size_t allocationSizeInBytes) const override;
									   			
			public: virtual struct HeapAllocation Allocate(size_t sizeInBytes) override;

				private: void CheckAllocationSize(size_t allocationSizeInBytes) const;
						
				private: void IncreaseFreeRegionOffset(size_t sizeInBytes);

						
			public: virtual void Reset() override;


			public: inline virtual DxPtr<ID3D12Heap> GetHeap() override { return heap; }
									
		};

		
	}
	
	
}
