#include <d3d12sdklayers.h>
#include <dxgi.h>
#include "DxPtrTypes.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"

namespace RHA
{
	namespace DX12
	{
		DeviceResourcesImpl::DeviceResourcesImpl()
		{
			{
				DxPtr<ID3D12Debug> debugController;
				auto result
				{
					D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))
				};

				if (FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create debug interface for dx12 device resources" };
				}

				debugController->EnableDebugLayer();
			}



			DxPtr<IDXGIFactory1> dxgiFactory;
			auto result
			{
				CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory))
			};

			if (FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not query dxgi factory" };
			}

			DxPtr<ID3D12Device> device;

			DxPtr<IDXGIAdapter1> hardwareAdapter;
			for (UINT adapterIndex{ 0 }; ; ++adapterIndex)
			{
				if (dxgiFactory->EnumAdapters1(adapterIndex, hardwareAdapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
				{
					continue;
				}

				auto result
				{
					D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device))
				};

				if (SUCCEEDED(result))
				{
					break;
				}
				hardwareAdapter->Release();

			}

			if (!device)
			{
				throw Exception::CreationFailed{ "Could not create dx12 device" };
			}

		}


	}


}
