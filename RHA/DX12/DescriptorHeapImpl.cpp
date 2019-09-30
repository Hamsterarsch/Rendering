#include "Shared/Exception/CreationFailedException.hpp"
#include "Shared/Exception/OutOfRangeException.hpp"
#include "DX12/DescriptorHeapImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		DescriptorHeapImpl::DescriptorHeapImpl
		(
			DeviceResources *resources,
			const D3D12_DESCRIPTOR_HEAP_TYPE type,
			const size_t capacity, 
			const bool isGpuVisible
		)
			: capacity{ capacity }
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};

			desc.Type = type;
			desc.NumDescriptors = capacity;
			desc.Flags = isGpuVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			auto result
			{
				resources->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap))
			};

			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 descriptor heap" };
			}

			heapStartCpu = heap->GetCPUDescriptorHandleForHeapStart();
			heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
			handleIncrementSize = resources->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);
						
		}

		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapImpl::GetHandleCpu(const size_t index)
		{
			if(index >= capacity)
			{
				throw Exception::OutOfRange{ "The targeted dx12 descriptor heap has not enough elements to access the specified index" };
			}

			return { heapStartCpu.ptr + index * handleIncrementSize };
			
		}

		D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapImpl::GetHandleGpu(const size_t index)
		{
			if (index >= capacity)
			{
				throw Exception::OutOfRange{ "The targeted dx12 descriptor heap has not enough elements to access the specified index" };
			}

			return { heapStartGpu.ptr + index * handleIncrementSize };
			
		}

		
	}

	
}
