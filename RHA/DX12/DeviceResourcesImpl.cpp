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

					const auto result
					{
						D3D12CreateDevice(hardwareAdapter.Get(), minimumFeatureLevel, IID_PPV_ARGS(&device))
					};

					if (SUCCEEDED(result))
					{
						break;
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
