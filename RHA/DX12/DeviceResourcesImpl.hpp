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
			const D3D_FEATURE_LEVEL minimumFeatureLevel;
			
			
			public: DeviceResourcesImpl(D3D_FEATURE_LEVEL minimumFeatureLevel, bool shouldEnableDebugLayers);
			
				private: void EnableDebugLayers();

					private: static void CheckDebugControllerCreation(HRESULT result);

				private: static void CheckDxgiFactoryCreation(HRESULT result);

				private: void CreateDeviceForFeatureLevel();

					private: void CheckDeviceCreation() const;

			public: virtual inline DxPtr<ID3D12Device> GetDevice() override { return device; }
			
			public: virtual inline DxPtr<IDXGIFactory> GetDxgiFactory() override { return dxgiFactory; }

			
		};


	}

	
}