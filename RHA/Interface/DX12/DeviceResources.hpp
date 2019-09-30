#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include "Shared/InterfaceHelpers.hpp"
#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResources
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DeviceResources)

			public: virtual DxPtr<ID3D12Device> GetDevice() = 0;
			public: virtual DxPtr<IDXGIFactory> GetDxgiFactory() = 0;

		};

		
	}

	
}
