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

			
			public: virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCpu(size_t index) = 0;
			
			public: virtual D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGpu(size_t index) = 0;

			
		};


	}


}
