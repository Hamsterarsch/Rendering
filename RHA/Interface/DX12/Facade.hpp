#pragma once
#include "DX12/DeviceResources.hpp"
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/WindowSurface.hpp"
#include "DX12/Fence.hpp"
#include "DX12/ShaderFactory.hpp"
#include "DX12/UploadHeap.hpp"
#include "DX12/Heap.hpp"
#include "Shared/PtrTypes.hpp"
#include "RHAExportHelper.hpp"
#include "DescriptorHeap.hpp"


namespace RHA
{
	namespace DX12
	{
		class RHA_DLLSPEC Facade
		{
		public:
			static UniquePtr<DeviceResources> MakeDeviceResources(D3D_FEATURE_LEVEL minimumFeatureLevel, bool shouldEnableDebugLayers);

			static UniquePtr<CmdAllocator> MakeCmdAllocator(DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type);

			static UniquePtr<Queue> MakeQueue(DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type, bool isHighPriority = false);

			static UniquePtr<WindowSurface> MakeWindowSurface(DeviceResources *resources, Queue *queue, HWND window);

			static UniquePtr<Fence> MakeFence(DeviceResources *resources);

			static UniquePtr<ShaderFactory> MakeShaderFactory(unsigned char shaderModelMajor, unsigned char shaderModelMinor);

			static UniquePtr<UploadHeap> MakeUploadHeap(DeviceResources *resources, size_t sizeInBytes);

			static UniquePtr<Heap> MakeHeap(DeviceResources *resources, size_t sizeInBytes, size_t alignment, D3D12_HEAP_FLAGS flags);

			static UniquePtr<DescriptorHeap> MakeDescriptorHeap(DeviceResources *resources, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t capacity, bool isGpuVisible);
			
		};

		
	}

	
}
