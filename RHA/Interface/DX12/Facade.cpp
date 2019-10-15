#include "Interface/DX12/Facade.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "DX12/CmdAllocatorImpl.hpp"
#include "DX12/QueueImpl.hpp"
#include "DX12/WindowSurfaceImpl.hpp"


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

		
	}

	
}
