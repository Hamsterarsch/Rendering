#pragma once
#include "Interface/DX12/DeviceResources.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResourcesImpl : public DeviceResources
		{
			DxPtr<IDXGIFactory1> dxgiFactory;
			DxPtr<ID3D12Device> device;
			
			public: DeviceResourcesImpl();

			public: virtual inline DxPtr<ID3D12Device> GetDevice() override { return device; }
			public: virtual inline DxPtr<IDXGIFactory> GetDxgiFactory() override { return dxgiFactory; }

			
		};


	}

	
}