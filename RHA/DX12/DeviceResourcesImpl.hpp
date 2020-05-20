#pragma once
#include "Interface/DX12/DeviceResources.hpp"


namespace RHA::DX12
{
	class DeviceResourcesImpl : public DeviceResources
	{
		private: DxPtr<IDXGIFactory4> dxgiFactory;
		
		private: DxPtr<ID3D12Device> device;
		
		private: const D3D_FEATURE_LEVEL minimumFeatureLevel;
		

		
		public: DeviceResourcesImpl(D3D_FEATURE_LEVEL minimumFeatureLevel, bool shouldEnableDebugLayers, bool shouldEnableGpuValidation);
		
			private: static void EnableDebugLayers(bool shouldEnableGpuValidation);

				private: static void CheckDebugControllerCreation(HRESULT result);

			private: static void CheckDxgiFactoryCreation(HRESULT result);

			private: void CreateDeviceForFeatureLevel();

				private: bool DoesDeviceSupportAllOptions() const;

				private: void CheckDeviceCreation() const;

		
		public: virtual inline DxPtr<ID3D12Device> GetDevice() override { return device; }
		
		public: virtual inline DxPtr<IDXGIFactory4> GetDxgiFactory() override { return dxgiFactory; }
							
	};

	
}