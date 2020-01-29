#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include <d3d12.h>


namespace RHA
{
	namespace DX12
	{
		class DepthSurface
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DepthSurface)
			

			public: virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCpu() const = 0;
						
		};

		
	}
	
	
}
