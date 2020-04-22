#pragma once
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"


namespace RHA
{
	namespace DX12
	{
		class DescriptorHeap
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DescriptorHeap)

			
			public: virtual ID3D12DescriptorHeap *GetHeap() = 0;
			
			public: virtual size_t Size() const = 0;
			
			public: virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCpu(size_t index) const = 0;
			
			public: virtual D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGpu(size_t index) const = 0;


			
		};


	}


}
