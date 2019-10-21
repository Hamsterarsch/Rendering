#include "Interface/DX12/Facade.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "DX12/CmdAllocatorImpl.hpp"
#include "DX12/QueueImpl.hpp"
#include "DX12/WindowSurfaceImpl.hpp"
#include "DX12/FenceImpl.hpp"
#include "DX12/ShaderFactoryImpl.hpp"
#include "DX12/UploadHeapImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		UniquePtr<DeviceResources> Facade::MakeDeviceResources
		(
			const D3D_FEATURE_LEVEL minimumFeatureLevel, 
			const bool shouldEnableDebugLayers
		)
		{
			return std::make_unique<DeviceResourcesImpl>(minimumFeatureLevel, shouldEnableDebugLayers);
			
		}

		UniquePtr<CmdAllocator> Facade::MakeCmdAllocator
		(
			DeviceResources *resources,
			D3D12_COMMAND_LIST_TYPE type
		)
		{
			return std::make_unique<CmdAllocatorImpl>(resources, type);
			
		}

		UniquePtr<Queue> Facade::MakeQueue
		(
			DeviceResources *resources, 
			D3D12_COMMAND_LIST_TYPE type
		)
		{
			return std::make_unique<QueueImpl>(resources, type);
			
		}

		UniquePtr<WindowSurface> Facade::MakeWindowSurface(DeviceResources *resources, Queue *queue, HWND window)
		{
			return std::make_unique<WindowSurfaceImpl>(resources, queue, window);
			
		}

		UniquePtr<Fence> Facade::MakeFence(DeviceResources *resources)
		{
			return std::make_unique<FenceImpl>(resources);
			
		}

		UniquePtr<ShaderFactory> Facade::MakeShaderFactory
		(
			unsigned char shaderModelMajor,
			unsigned char shaderModelMinor
		)
		{
			return std::make_unique<ShaderFactoryImpl>(shaderModelMajor, shaderModelMinor);
			
		}

		UniquePtr<UploadHeap> Facade::MakeUploadHeap(DeviceResources *resources, size_t sizeInBytes)
		{
			return std::make_unique<UploadHeapImpl>(resources, sizeInBytes);
			
		}

		
	}

	
}
