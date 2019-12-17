#include "Interface/DX12/Facade.hpp"
#include "DX12/DeviceResourcesImpl.hpp"
#include "DX12/CmdAllocatorImpl.hpp"
#include "DX12/QueueImpl.hpp"
#include "DX12/WindowSurfaceImpl.hpp"
#include "DX12/FenceImpl.hpp"
#include "DX12/ShaderFactoryImpl.hpp"
#include "DX12/UploadHeapImpl.hpp"
#include "Dx12/HeapImpl.hpp"


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
			const D3D12_COMMAND_LIST_TYPE type,
			const bool isHighPriority
		)
		{
			return std::make_unique<QueueImpl>(resources, type, isHighPriority);
			
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

		UniquePtr<Heap> Facade::MakeHeap(DeviceResources* resources, size_t sizeInBytes, size_t alignment, D3D12_HEAP_FLAGS flags)
		{
			return std::make_unique<HeapImpl>(resources, sizeInBytes, alignment, flags);
			
		}

		UniquePtr<DescriptorHeap> Facade::MakeDescriptorHeap
		(
			DeviceResources *resources,
			const D3D12_DESCRIPTOR_HEAP_TYPE type,
			const size_t capacity,
			const bool isGpuVisible
		)
		{
			return std::make_unique<DescriptorHeapImpl>(resources, type, capacity, isGpuVisible);
			
		}

		
	}

	
}
