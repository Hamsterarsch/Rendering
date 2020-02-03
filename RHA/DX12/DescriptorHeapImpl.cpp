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

			const auto result
			{
				resources->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap))
			};
			CheckHeapCreation(result);

			heapStartCpu = heap->GetCPUDescriptorHandleForHeapStart();
			heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
			handleIncrementSize = resources->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);
						
		}

			void DescriptorHeapImpl::CheckHeapCreation(HRESULT result)
			{
				if (FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 descriptor heap" };
				}
			
			}


		
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapImpl::GetHandleCpu(const size_t index) const
		{
			HandleCorruptIndex(index);

			return { heapStartCpu.ptr + index * handleIncrementSize };
			
		}

			void DescriptorHeapImpl::HandleCorruptIndex(const size_t index) const
			{
				if (IndexIsInvalid(index))
				{
					throw Exception::OutOfRange{ "The targeted dx12 descriptor heap has not enough elements to access the specified index" };
				}
			
			}

				bool DescriptorHeapImpl::IndexIsInvalid(const size_t index) const
				{
					return index >= capacity;
				
				}


		
		D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapImpl::GetHandleGpu(const size_t index) const
		{
			HandleCorruptIndex(index);

			return { heapStartGpu.ptr + index * handleIncrementSize };
			
		}

		
	}

	
}
