#include "DxPtrTypes.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"

namespace RHA
{
	namespace DX12
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
				const auto result
				{
					D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))
				};

				CheckDebugControllerCreation(result);

				debugController->EnableDebugLayer();
			
			}

				void DeviceResourcesImpl::CheckDebugControllerCreation(HRESULT result) 
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not create debug interface for dx12 device resources" };
					}
				
				}

			void DeviceResourcesImpl::CheckDxgiFactoryCreation(HRESULT result)
			{
				if (FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not query dxgi factory" };
				}
				
			}

			void DeviceResourcesImpl::CreateDeviceForFeatureLevel()
			{
				DxPtr<IDXGIAdapter1> hardwareAdapter;
				for (UINT adapterIndex{ 0 }; ; ++adapterIndex)
				{
					if (dxgiFactory->EnumAdapters1(adapterIndex, hardwareAdapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)//exit when there are no more adapters to iterate
					{
						continue;
					}

					DXGI_ADAPTER_DESC1 desc; hardwareAdapter->GetDesc1(&desc);

					const auto result
					{
						D3D12CreateDevice(hardwareAdapter.Get(), minimumFeatureLevel, IID_PPV_ARGS(&device))
					};


					if (SUCCEEDED(result))
					{
						D3D12_FEATURE_DATA_D3D12_OPTIONS options;
						device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(decltype(options)) );
						if(options.TiledResourcesTier > D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED)
						{
							break;								
						}						
					}					
					hardwareAdapter->Release();

				}
				CheckDeviceCreation();
			
			}

				void DeviceResourcesImpl::CheckDeviceCreation() const
				{
					if (!device)
					{
						throw Exception::CreationFailed{ "Could not create dx12 device" };
					}
			
				}


	}


}
