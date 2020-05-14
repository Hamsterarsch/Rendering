#include "DxPtrTypes.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"

namespace RHA::DX12
{
	DeviceResourcesImpl::DeviceResourcesImpl(const D3D_FEATURE_LEVEL minimumFeatureLevel, const bool shouldEnableDebugLayers) :
		minimumFeatureLevel{ minimumFeatureLevel }
	{
		if(shouldEnableDebugLayers)
		{
			EnableDebugLayers();
		}
		
		const auto result
		{
			CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory))
		};
		CheckDxgiFactoryCreation(result);
					
		CreateDeviceForFeatureLevel();
				   
	}

		void DeviceResourcesImpl::EnableDebugLayers()
		{
			DxPtr<ID3D12Debug> debugController;
			DxPtr<ID3D12Debug1> debugController1;
			auto result
			{
				D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))
			};
			CheckDebugControllerCreation(result);

			result = debugController->QueryInterface(IID_PPV_ARGS(&debugController1));
			CheckDebugControllerCreation(result);

			debugController->EnableDebugLayer();
			debugController1->SetEnableGPUBasedValidation(true);
		
		}

			void DeviceResourcesImpl::CheckDebugControllerCreation(const HRESULT result) 
			{
				if (FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create debug interface for dx12 device resources" };
				}
			
			}

		void DeviceResourcesImpl::CheckDxgiFactoryCreation(const HRESULT result)
		{
			if (FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not query dxgi factory" };
			}
			
		}

		void DeviceResourcesImpl::CreateDeviceForFeatureLevel()
		{
			for (UINT adapterIndex{ 0 }; ; ++adapterIndex)
			{
				DxPtr<IDXGIAdapter1> hardwareAdapter;
				dxgiFactory->EnumAdapters1(adapterIndex, hardwareAdapter.GetAddressOf());
				if (hardwareAdapter == nullptr)
				{						
					break;
				}
														
				const auto result
				{
					D3D12CreateDevice(hardwareAdapter.Get(), minimumFeatureLevel, IID_PPV_ARGS(&device))
				};
				
				if (SUCCEEDED(result) && DoesDeviceSupportAllOptions())
				{
					break;
				}					
				
			}
			CheckDeviceCreation();
		
		}

			bool DeviceResourcesImpl::DoesDeviceSupportAllOptions() const
			{
				D3D12_FEATURE_DATA_D3D12_OPTIONS options;
				device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(decltype(options)) );
				
				return options.TiledResourcesTier > D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED;
		
			}

			void DeviceResourcesImpl::CheckDeviceCreation() const
			{
				if (!device)
				{
					throw Exception::CreationFailed{ "Could not create dx12 device with tiled resource support" };
				}
		
			}


}
