#pragma once
#include "DX12/DeviceResources.hpp"
#include <d3d12.h>
#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class DescriptorHeapImpl
		{
			private: DxPtr<ID3D12DescriptorHeap> heap;
			private: size_t handleIncrementSize;
			private: size_t capacity;
			private: D3D12_CPU_DESCRIPTOR_HANDLE heapStartCpu;
			private: D3D12_GPU_DESCRIPTOR_HANDLE heapStartGpu;
		
			
			//object creation is not thread safe: we assume that a weak resource ref must stay valid at all times during object construction
			public: DescriptorHeapImpl(DeviceResources *resources, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t capacity, bool isGpuVisible);

				private: static void CheckHeapCreation(HRESULT result);
			
			public: D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCpu(size_t index);

				private: void HandleCorruptIndex(size_t index) const;
			
					private: bool IndexIsInvalid(size_t index) const;
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGpu(size_t index);

			
		};

		
	}

	
}
